imnotgui와 다른 점
1. iui_*_alpha 계열 함수 제거: raylib::Color 클래스는 기본적으로 알파값을 포함합니다.
2. iui_label 계열 함수를 ELEMENT 그룹에서 DRAW 그룹으로 이동: 사용자와 상호작용하지 않는 요소이므로 DRAW 그룹의 다른 함수와 유사한 특징을 공유한다고 보았습니다.
또한 백엔드 종속성을 ELEMENT에서 분리하기 위해서도 필요합니다.
3. strTrim_nodots: 함수 이름이 iui_strTrimNodots로 변경되었으며, 최대 길이 -3이었던 부분을 -1로 변경했습니다.
4. iui_tab: line 128과 line 129에서 같은 함수가 중복 작성되어 있는 사소한 버그를 수정했습니다.
5. iui_tab과 iui_get_all에서 참조자 파라미터를 사용하는 방향으로 작성했습니다.
~~6. iui_align_* 계열 함수를 삭제했습니다. UI 디자인에서 alignment stack이 무엇을 달성할 수 있는지 확실하지 않습니다.
그러나 이는 개인적인 의견으로, 추후 롤백되거나 바뀔 수 있습니다.~~
-> 모든 드로우 함수를 stateless하게 만들기 위해 인자가 지나치게 많아집니다.
stack 구조를 삭제하고 iui_setAlignment 등을 추가했습니다.

7. iui_label_transform 함수가 iui_label_scale 함수로 바뀌면서 xscale과 yscale이 하나로 통합되었습니다.
raylib에서 자유 비율을 제공하지 않으며, 일반적으로 이러한 기능이 필요하지 않은 것처럼 보입니다.
추후 필요하다면 추가될 수 있습니다.

8. varmap을 삭제했습니다. 이 자료구조는 textbox와 slider에서만 사용되는데, 두 경우 모두 varmap을 사용하지 않는 방향으로 코드를 수정했습니다.
9. iui_textbox_numberonly를 iui_intbox와 iui_floatbox로 나누었습니다. 입력 텍스트가 해석 가능하다면 레퍼런스로 결과값을 저장합니다. 유효하지 않은 입력은 가능한 한(best-effort) 금지합니다.
9. iui_*box 함수의 리턴 타입을 void에서 bool로 변경했습니다. focused 상태에서 enter를 누르면 포커스가 해제되며 true를 반환합니다.

10. 현재 iui_slider_* 함수는 int형 변수만 지원합니다. 함수 오버로딩을 통해 구현해야 합니다.


Further updates
- 한글 폰트 지원과 함께 textbox 한글 타이핑을 지원해야 합니다.
