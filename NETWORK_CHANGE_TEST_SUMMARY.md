# 네트워크 변경 이유와 테스트 정리

기준 문서:
- `progress_issues.md`
- `train_code/기본코드/Depth_Profile_Tip_v3/MODEL_STRUCTURE.md`

작성일: 2026-06-12

## 1. 현재 목표

이 프로젝트의 네트워크 목표 우선순위는 다음과 같이 정리되어 있다.

1. `TipH` 반복도와 GT 정밀도
2. `Depth map` 복원 품질
3. `tip_x` 위치
4. `profile` 보조 품질

즉, 단순히 tip 위치가 잘 맞거나 profile이 그럴듯한 것보다, 최종 검사값으로 쓰이는 `TipH`가 반복 측정에서 안정적이고 GT 정의를 잘 따라가는 것이 가장 중요하다.

## 2. 바꾸게 된 핵심 문제

초기 분석에서 네트워크 내부의 depth/profile 정보 자체는 비교적 살아 있었지만, `TipH` 출력 head가 그 정보를 안정적으로 쓰지 못했다.

관측된 문제는 크게 네 가지였다.

| 문제 | 관측 내용 | 결론 |
| --- | --- | --- |
| Direct TipHeightHead 저평가 | 기존 direct head가 TEST2에서 `TipH`를 크게 낮게 예측했다. `best.pth`는 약 `-50`, `Epoch_28.pth`는 약 `-61` bias가 있었다. | 높이 정보가 없는 것이 아니라 direct head가 잘못 읽고 있었다. |
| `profile@GT`는 거의 무편향 | 같은 checkpoint에서 `profile@GT`는 bias가 작았다. `Epoch_28.pth` 기준 `profile@GT` bias는 약 `-0.24`였다. | profile 안에는 높이 정보가 이미 들어 있었다. |
| 반복도 metric mismatch | 학습 중 `TipH 5pct Repeat Proxy`가 좋아도 `repeat_ui.py` / `infer_compare_OnnxPth.py` 최종 반복도는 낮게 나올 수 있었다. | 학습 proxy는 최종 0~49 반복 평가를 대체하지 못한다. |
| 낮은칩 Black 반복도 실패 | path diagnostic groupby에서 낮은칩 Black은 pre-align / direction-aware 경로 모두 한 자릿수 수준 RangePass였다. | 경로 선택만으로는 해결되지 않고, 낮은칩 boundary profile 반복도를 직접 개선해야 한다. |

## 3. 변경 타임라인

| 단계 | 날짜 | 변경 | 이유 | 테스트/근거 |
| --- | --- | --- | --- | --- |
| TipH 우선순위 반영 | 2026-06-08 | `TipH` 관련 loss 가중치 강화 | 사용자가 `TipH` 반복도와 GT 정밀도를 최우선으로 지정 | TipHead 진단 스크립트 추가, 기존 checkpoint 기준 `tipH_v3` range 개선 확인 |
| robust TipHead / TipHeightHead 추가 | 2026-06-08 | `TipHead`, `TipHeightHead`, TipH-first training loss 추가 | 기존 tip 출력이 불안정하고 높이 직접 출력이 필요했음 | syntax check, dummy forward 통과 |
| `profile(tip_x)` identity 경로 분리 | 2026-06-09 | TipH가 `tip_x` 샘플링에 직접 끌려가지 않도록 detach 기반 direct head로 변경 | 경사면에서 작은 `tip_x` jitter가 큰 height variation으로 증폭됨 | direct TipHeightHead forward / compile 통과 |
| plateau/max fallback 금지 규칙 확정 | 2026-06-09 | 낮은칩 TipH target을 max/plateau로 임의 대체하지 않음 | 낮은칩도 납이 항상 칩 위로 올라타는 것은 아니고, 변곡점/경계 정의가 물리적으로 중요함 | `TipH Target Rules`와 progress issue에 기록 |
| re-anchor 구조 도입 | 2026-06-10 | `TipHeightHead = local p25 profile anchor + bounded residual` | profile 안에 높이 정보가 이미 존재하므로, direct head가 절대 높이를 새로 맞히지 않고 profile anchor 주변 보정만 하도록 변경 | `Epoch_28.pth`에서 zero residual 기준 TEST2 direct bias가 약 `+1.55`로 이동 |
| fallback target 약화 | 2026-06-10 | `orig_g_h` 없는 fallback height supervision을 `0.30x`로 약화 | fallback target이 direct head에 혼합분포를 만들 수 있음. 다만 낮은칩 supervision을 완전히 없애면 위험함 | target source logging 추가 |
| TipH error 출력 추가 | 2026-06-10 | train/valid TipH MAE, Bias, Std, 3sigma, orig/fallback MAE 출력 | checkpoint 선택 기준이 valid final loss만이면 TipH 목표와 어긋날 수 있음 | Solder Python compile 통과 |
| 5pct repeat proxy 추가 | 2026-06-10 | sibling TipH max-min/3sigma를 `max(profile)*0.05` 기준으로 출력 | repeat UI의 판정 방향과 맞춘 학습 중 참고 지표 필요 | compile 통과. 단, final repeat evaluation 대체 불가로 기록 |
| infer repeat summary 추가 | 2026-06-11 | `infer_compare_OnnxPth.py` 종료 시 repeat-ui-compatible summary 출력 | 실제 0~49 반복 결과를 바로 확인하기 위함 | `D:/data2/data2` 기준 RangePass 약 `38.2%`, 3sigmaPass 약 `64.5%` 확인 |
| path diagnostic 추가 | 2026-06-11 | pre-align `dirs=[1]` vs direction-aware `dirs=[folder_dir]` 비교 모드 추가 | dir4에서 경로에 따라 반복도가 크게 달라지는 현상 확인 필요 | 전체 220 folders CSV 분석 |
| pre-align 유지 결정 | 2026-06-11 | 기본 추론 경로는 pre-align 유지 | direction-aware는 RangePass `79 -> 81` 정도로 미미하고 3sigmaPass는 `130 -> 123`으로 악화 | dual-path runtime selection은 검사장비 1회 추론 원칙에 맞지 않아 금지 |
| boundary profile / residual consistency 추가 | 2026-06-11 | tip 주변 profile window consistency, residual consistency, residual L1 추가 | worst-folder 분해에서 `tip_x`보다 boundary profile shape와 residual 증폭이 지배적이었음 | compile, dummy forward, `best.pth` strict load 통과 |

## 4. 현재 네트워크 구조 요약

현재 핵심 경로는 다음과 같다.

```text
inputs (B,9,H,W)
  -> direction-aware guide U-Net backbone
  -> depth map
  -> profile_head
  -> TipHead
  -> branch-gap stable tip selector
  -> TipHeightHead
  -> tip_height
```

`TipHeightHead`는 `TipH`를 완전 direct regression으로 새로 만들지 않는다.

현재 구조:

```text
anchor_h =
    0.90 * local_p25_read(profile.detach(), tip_x.detach(), radius=12)
  + 0.10 * center_read(profile.detach(), tip_x.detach())

residual =
    tanh(residual_fc(local features)) * residual_bound

tip_height =
    clamp(anchor_h + residual)
```

이 구조를 재앵커링이라고 부른다. 의미는 다음과 같다.

- profile 안에 이미 존재하는 높이 정보를 기준점(anchor)으로 사용한다.
- head는 절대 높이를 처음부터 다시 맞히지 않고, anchor 주변의 작은 보정(residual)만 학습한다.
- `profile`과 `tip_x`는 anchor 계산에서 `detach()`되어 TipH loss가 profile/tip_x를 직접 끌고 가지 않게 한다.
- `local p25`는 plateau/max fallback이 아니라, tip 주변 window 안에서 높은 solder plateau에 덜 끌리는 robust local readout이다.

## 5. 현재 출력 형태

학습 경로 `details=False`:

| 출력 | 형태 | 의미 |
| --- | --- | --- |
| `out` | `(B,2,H,W)` 또는 `(B,1,H,W)` | depth와 optional uncertainty |
| `profile_out` | `(B,1,L)` | canonical 1D profile |
| `scalar_out` | `(B,8)` | tip 관련 학습 bundle |

`scalar_out` 구성:

```text
scalar_out[:, 0] = tip_x
scalar_out[:, 1] = tip_height
scalar_out[:, 2] = pos_offset
scalar_out[:, 3] = pos_seg
scalar_out[:, 4] = pos_heatmap
scalar_out[:, 5] = gate_conf
scalar_out[:, 6] = anchor_h
scalar_out[:, 7] = residual
```

추론/ONNX 경로 `details=True`:

```text
depth, profile_out, tip_x, tip_height
```

주의: 2026-06-11 변경에서 학습용 `scalar_out`만 8채널로 늘었고, 추론 출력 4개는 유지했다. 검사장비 runtime 출력 shape는 바꾸지 않았다.

## 6. 왜 residual bound를 줄이지 않았나

분해 결과에서 residual은 현재 bound 전체를 쓰는 상태가 아니었다.

- residual std: 약 `3.66`
- residual range: 약 `20`
- 현재 bound: `0.05`, 물리 단위로 약 `±250`

즉 residual은 bound의 작은 일부만 쓰고 있었으므로, bound를 `0.02`로 줄여도 실제 동작을 거의 묶지 못한다. 그래서 bound 축소 대신 다음 두 항을 추가했다.

| 항목 | 목적 |
| --- | --- |
| residual sibling consistency | 같은 부품 반복 이미지에서 residual이 서로 다르게 흔들리지 않도록 제약 |
| residual L1 | residual 자체를 0 쪽으로 약하게 당겨 anchor noise를 따라 증폭하지 않게 함 |

## 7. 왜 boundary profile consistency를 추가했나

`1005_350_Black_180d_40_3` worst-folder 분해에서 다음 결과가 나왔다.

| 항목 | 값 | 해석 |
| --- | --- | --- |
| 최종 TipH std | 약 `10.91` | 3sigma 기준으로 5pct band를 넘음 |
| `tip_x` std | 약 `0.68 px` | 생각보다 안정적 |
| 위치 기여 | 약 `3.2` | 전체 흔들림의 주범이 아님 |
| anchor@pred_x std | 약 `8.08` | anchor 자체가 크게 흔들림 |
| anchor@fixed_x std | 약 `7.59` | 위치를 고정해도 거의 그대로 흔들림 |
| corr(TipH, anchor@fixed) | 약 `+0.916` | boundary profile shape가 최종 TipH를 지배 |
| residual std | 약 `3.66` | 작지만 최종 TipH와 같은 방향으로 흔들림 |
| corr(TipH, residual) | 약 `+0.84` | residual이 보정이 아니라 증폭처럼 동작 |

따라서 selector나 `tip_x`만 더 안정화하는 것은 우선순위가 낮아졌다. 현재 문제의 중심은 낮은칩 boundary 주변 profile shape 반복도와 residual 증폭이다.

추가된 loss:

```text
loss_tiprep =
    lam_tip_pos           * L_pos
  + lam_tip_h             * L_h
  + lam_tip_profile       * L_boundary_profile
  + lam_tip_residual_cons * L_residual_consistency
  + lam_tip_residual_l1   * L_residual_l1
```

현재 값:

| 변수 | 값 | 의미 |
| --- | --- | --- |
| `lam_tip_pos` | `0.05` | tip_x 반복도는 보조 |
| `lam_tip_h` | `0.95` | TipH 반복도 핵심 |
| `lam_tip_profile` | `0.35` | tip 주변 ±12 profile window sibling consistency |
| `lam_tip_residual_cons` | `0.50` | residual sibling consistency |
| `lam_tip_residual_l1` | `0.10` | residual 크기 L1 |

이 전체 `tiprep` 항은 기존 warm-up을 그대로 탄다. 초반에 GT bias가 아직 잡히기 전에 boundary를 너무 강하게 뭉개지 않기 위해서다.

## 8. path diagnostic에서 배운 점

`infer_compare_OnnxPth.py --diagnose-paths`로 두 경로를 비교했다.

| 경로 | 의미 |
| --- | --- |
| `prealign_dirs1` | 현재 기본 infer 경로. 원본 방향을 dir1 canonical 형태로 pre-align하고 `dirs=[1]`로 추론 |
| `direction_aware` | pre-align 없이 원본 방향을 넣고 `dirs=[folder_dir]`로 추론 |

전체 220 folders 결과:

| 항목 | pre-align | direction-aware |
| --- | --- | --- |
| RangePass | `79/220` | `81/220` |
| 3sigmaPass | `130/220` | `123/220` |
| mean MaxMin | `36.76` | `36.30` |

결론:

- 경로별로 좋아지는 폴더와 나빠지는 폴더가 섞인다.
- dir4 일부 폴더는 direction-aware가 크게 좋아졌지만, 전체 정책으로 바꾸기에는 근거가 약하다.
- 검사장비는 샘플당 1회 추론이 원칙이므로 dual-path를 둘 다 돌려 좋은 쪽을 고르는 정책은 금지한다.
- 현재는 pre-align 경로를 유지하고, 모델이 경로/경계 흔들림에 덜 민감해지도록 학습 쪽을 개선한다.

## 9. 주요 테스트 기록

| 테스트 | 결과 | 의미 |
| --- | --- | --- |
| TEST2 GT/profile sanity | profile MAE 약 `1.019`, tipH diff mean 약 `-2.039` | GT/profile 추출 자체는 큰 문제 없어 보임 |
| direct head quick check | direct TipH bias `-50` ~ `-69` 수준 | direct head가 높이를 낮게 치우쳐 출력 |
| `profile@GT` check | `Epoch_28.pth` 기준 bias 약 `-0.24` | profile 안 높이 정보는 존재 |
| re-anchor candidate check | local p25 bias 약 `-11.40` | p25가 plateau/max 없이 안정적인 anchor 후보 |
| re-anchor zero residual check | TEST2 direct TipH bias 약 `+1.55` | direct head bias 문제를 크게 완화 |
| re-anchor restore check | 기존 `Epoch_29.pth`에서 direct TipH bias 약 `+1.10`, std 약 `34.42` | 구조 복원 후 정상 동작 |
| actual Deep repeat summary | RangePass 약 `38.2%`, 3sigmaPass 약 `64.5%` | 평균은 일부 개선되나 낮은칩 tail이 문제 |
| path diagnostic full CSV | direction-aware RangePass `79 -> 81`, 3sigmaPass `130 -> 123` | 경로 전환만으로 해결 불가 |
| worst-folder decomposition | fixed-x anchor std 약 `7.59`, residual corr 약 `+0.84` | boundary profile shape와 residual 증폭이 병목 |
| boundary/residual 변경 후 compile | Pass | 문법 오류 없음 |
| dummy forward | Pass, train scalar `(B,8)`, details output 4 tensors | 학습 출력 확장, 추론 출력 유지 |
| `best.pth` strict load | Pass | checkpoint 호환성 유지 |

## 10. 현재 미해결 리스크

| 리스크 | 이유 | 대응 |
| --- | --- | --- |
| anchor 변경으로 bias가 음/양으로 이동할 수 있음 | `0.90*p25 + 0.10*center`는 기존보다 p25 쪽에 더 치우침 | 짧게 재학습 후 TipH MAE/Bias/Std 확인 |
| 학습 proxy가 실제 repeat와 다를 수 있음 | sibling proxy는 최종 0~49 반복 평가와 완전히 같지 않음 | `infer_compare_OnnxPth.py`와 `repeat_ui.py` 최종 평가 유지 |
| 낮은칩 불량/작은 solder 영역은 변곡점도 불안정할 수 있음 | 사용자가 plateau/max fallback을 금지했지만, 일부 예외 케이스는 물리 정의 자체가 어려움 | confidence/exception 판정은 별도 이슈로 유지 |
| path mismatch가 완전히 사라진 것은 아님 | pre-align과 direction-aware 사이 출력 차이가 존재 | runtime은 single path 유지, path diagnostic은 offline 진단으로만 사용 |

## 11. 다음 검증 체크리스트

학습 후 다음 순서로 확인한다.

1. 학습 로그
   - `TipH Error`: MAE, Bias, Std, 3sigma
   - `TipH 5pct Repeat Proxy`: RangePass, 3sigmaPass
   - fallback/orig MAE 차이

2. 실제 반복 평가
   - `infer_compare_OnnxPth.py` 실행
   - `repeat_ui.py`로 Deep 반복도 확인
   - 낮은칩 Black 그룹을 별도로 확인

3. worst-folder 재분해
   - `1005_350_Black_180d_40_3`
   - `1005_350_Black_180d_16_4`
   - 필요 시 `0916_450_BlackO` 계열

4. 분산 예산 비교
   - 목표는 fixed-position anchor std를 낮추는 것
   - residual std와 TipH-residual correlation이 줄었는지 확인
   - `tip_x`만 좋아졌는데 TipH가 그대로면 이번 변경 목적을 달성하지 못한 것

5. checkpoint 선택
   - valid final loss만 보지 않는다.
   - TipH Bias/Std, repeat-ui-compatible RangePass/3sigmaPass를 함께 본다.

## 12. 한 줄 결론

지금까지의 네트워크 변경은 `TipH`를 완전 direct regression으로 맞히려던 방향에서, 이미 profile에 들어있는 높이 정보를 anchor로 삼고 residual은 작은 보정만 하도록 제한하는 방향으로 이동했다. 최신 변경은 낮은칩 Black에서 반복도가 무너지는 원인이 `tip_x`보다 boundary profile shape와 residual 증폭에 있다는 분해 결과를 반영해, 학습 단계에서 그 두 항목을 직접 제약하도록 만든 것이다.
