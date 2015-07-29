# Triton-windows

https://github.com/JonathanSalwan/Triton

JonathanSalwan이 만든 Triton을 Windows용으로 짠것입니다.
본래 목적과는 다른 목적으로 만들었기 때문에 몇몇 기능은 존재하지 않습니다.
(SnapshotEngine, Python Binding, boost::filesystem) 제외
또한 boost라이브러리를 쓰지 않아 128bit 변수는 64bit 변수로 대체되었습니다.

포팅을 허락해준 JonathanSalwan에게 감사를 표합니다.

본 소스는 concolic execution의 Pin 기반 Framework 입니다.
크게 Taint Engine, Symbolic Engine, SMT - LIB로 이루어져있고,
버그 및 Triton 이슈사항은 올려주시면 감사히 수정하겠습니다.

example 을 보시면 간단히 사용하는 방법이 올라와 있으니 많은 이슈 올려주셨으면 좋겠습니다. ㅎㅎ


