# Порядок именования переменных, функций, структур, enum и классов.
## Переменные
1. Имя переменной начинается с маленькой буквы, далее в `lowerCamelCase`. **Пример: someVariable**;
2. Константы пишутся капсом с разделителем `_`. **Пример: SOME_CONSTANT**;
3. private переменные пишутся с постфиксом `_`. **Пример: someVariable_**;
4. Булевые переменные должны начинаться с префикса is (can/has/should etc.). **Пример: isVisible**.

## Функции
1. Имена функций начинаются с глагола, далее в `lowerCameCase`. **Пример: doSomeThing()**;
2. Аббревиатуры и сокращения в имени записываются в нижнем регистре;
3. Входные переменные в set-функциях именуются так же, как и те, которые они изменяют;
4. get/set, add/remove, create/destroy, start/stop, insert/delete, increment/decrement, old/new, begin/end, first/last, up/down, min/max, next/previous, old/new, open/close, show/hide, suspend/resume.
    
## Структуры, enum, классы
1. Cтруктуры именуются в `UpperCamelCase`. **Пример: SomeStructure**;
2. Enum'ы именуются аналогично структурам;
3. Значения enum'ов именуются в `under_score`. **Пример: some_value**
4. Значения enum'ов следует использовать с именем самого enum'а. **Пример: QEnum::enum**;
5. Если класс представляет из себя интерфейс (абстрактный класс), то он начинается с буквы `I`, далее в `UpperCamelCase`. **Пример: IClass**;
6. Если класс не является интерфейсом (абстракным классом), то он именуется в `UpperCamelCase`. **Пример: SomeClass**.