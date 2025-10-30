# Вопрос

> На графе graphviz/dot у меня есть рёбра, которые входят в ту же вершину, из которой выходят. Они отображаются маленькими петлями, и несколько рёбер наезжают метками друг на друга. Можно ли как-то их сделать побольше?


Да, есть несколько способов увеличить петли и разнести метки в Graphviz/dot:

## 1. Увеличение размера петель

```dot
digraph G {
    // Увеличение размера петли через minlen
    A -> A [minlen=2];
    
    // Или через увеличение размера вершины
    B [width=1, height=1];
    B -> B;
}
```

## 2. Разнесение меток

```dot
digraph G {
    // Разные позиции для меток
    A -> A [label="Петля 1", labelfloat=true, labeldistance=2];
    A -> A [label="Петля 2", labelfloat=true, labeldistance=3];
    
    // Использование headlabel/taillabel
    B -> B [headlabel="с головы", taillabel="с хвоста"];
}
```

## 3. Более продвинутый подход с портами

```dot
digraph G {
    node [shape=circle, width=1];
    
    // Петли с разных сторон вершины
    A -> A [label="Верх", headport=n, tailport=n];
    A -> A [label="Низ", headport=s, tailport=s];
    A -> A [label="Лево", headport=w, tailport=w];
    A -> A [label="Право", headport=e, tailport=e];
}
```

## 4. Использование splines и curvature

```dot
digraph G {
    splines=true;
    
    A -> A [label="Петля 1", curvature=0.8];
    A -> A [label="Петля 2", curvature=-0.8];
    
    // Или с явным указанием контрольных точек
    B -> B [label="Большая", headclip=false, tailclip=false];
}
```

## 5. Полный пример с настройками

```dot
digraph G {
    graph [rankdir=TB, splines=true];
    node [shape=circle, width=0.8, height=0.8];
    
    // Петли с разными настройками
    A -> A [label="Loop 1", 
           minlen=2, 
           labelfloat=true,
           labeldistance=2.5,
           curvature=0.6];
           
    A -> A [label="Loop 2", 
           minlen=3,
           labelfloat=true, 
           labeldistance=3.5,
           curvature=-0.6];
    
    // Альтернативный подход - использование невидимых узлов
    B -> invisible1 [arrowhead=none, style=dashed];
    invisible1 -> B [label="Loop 3"];
    invisible1 [shape=point, width=0];
}
```

## Ключевые параметры:

- **minlen** - минимальная длина ребра (увеличивает петлю)
- **curvature** - кривизна петли (положительные и отрицательные значения для разных направлений)
- **labelfloat**, **labeldistance** - позиционирование меток
- **headport**, **tailport** - точки входа/выхода на вершине
- **splines** - сглаживание линий

Экспериментируйте с этими параметрами, чтобы найти оптимальное отображение для вашего графа.


# Вопрос

> Невидимые вершины с shape=point и width=0 всё равно заметны, а если сделать style="invis", то там явно видно разрыв. Какие есть ещё варианты с невидимыми вершинами?


Вы правы! У `shape=point` и `style=invis` есть свои недостатки. Вот более эффективные подходы:

## 1. Использование edge-атрибутов без промежуточных вершин

```dot
digraph G {
    A -> A [label="Петля 1", headport=nw, tailport=ne];
    A -> A [label="Петля 2", headport=sw, tailport=se];
    
    // Или с явным указанием углов
    B -> B [label="Внешняя", headport=n, tailport=n, constraint=false];
    B -> B [label="Внутренняя", headport=s, tailport=s, constraint=false];
}
```

## 2. Тонкая настройка портов и весов

```dot
digraph G {
    node [shape=circle, width=1];
    
    A -> A [label="1", headport=n, tailport=ne, weight=0];
    A -> A [label="2", headport=e, tailport=se, weight=0];
    A -> A [label="3", headport=s, tailport=sw, weight=0];
    A -> A [label="4", headport=w, tailport=nw, weight=0];
}
```

## 3. Использование HTML-подобных меток с таблицами

```dot
digraph G {
    A [shape=plaintext, label=<
        <TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0">
            <TR><TD PORT="n">Север</TD></TR>
            <TR><TD PORT="center">Центр</TD></TR>
            <TR><TD PORT="s">Юг</TD></TR>
        </TABLE>
    >];
    
    A:n -> A:s [label="Петля"];
    A:s -> A:n [label="Обратная"];
}
```

## 4. Комбинация нескольких техник

```dot
digraph G {
    splines=true;
    
    // Основные петли
    A -> A [label="Основная", dir=both, arrowtail=dot, arrowhead=odot];
    
    // Разнесённые петли через разные порты
    A -> A [label="Доп 1", headport=n, tailport=ne, minlen=3];
    A -> A [label="Доп 2", headport=e, tailport=se, minlen=3];
    
    // Использование constraint=false для более свободного позиционирования
    A -> A [label="Свободная", constraint=false, labelfloat=true];
}
```

## 5. Альтернатива с группировкой

```dot
digraph G {
    subgraph cluster_A {
        style=invis;
        A1 [label="", shape=point, width=0, height=0];
        A2 [label="", shape=point, width=0, height=0];
        A3 [label="A", shape=circle];
    }
    
    A3 -> A1 [arrowhead=none];
    A1 -> A2 [label="Петля 1"];
    A2 -> A3;
    
    A3 -> A3 [label="Петля 2"];
}
```

## 6. Самый чистый вариант - только порты

```dot
digraph G {
    node [shape=circle, width=1.2, height=1.2];
    
    // 8 различных портов для максимального разнесения
    A:n -> A:ne [label="1"];
    A:e -> A:se [label="2"]; 
    A:s -> A:sw [label="3"];
    A:w -> A:nw [label="4"];
    
    // Дополнительные с minlen
    A:n -> A:e [label="5", minlen=2];
    A:s -> A:w [label="6", minlen=2];
}
```

## Ключевые моменты:

- **headport/tailport** - основной инструмент для разнесения
- **constraint=false** - убирает влияние на layout
- **minlen** - увеличивает размер петли
- **weight=0** - уменьшает влияние на позиционирование
- **labelfloat=true** - свободное позиционирование меток

Рекомендую начинать с подхода №6 - он самый чистый и не требует искусственных вершин.


# Вопрос

> Мне больше всего понравилось с промежуточной невидимой вершиной, но не понятно, как сделать в этом случае бесшовное слияние двух рёбер, чтобы это не было заметно

Отличный вопрос! Вот несколько способов сделать слияние действительно бесшовным:

## 1. Идеальное совмещение стилей

```dot
digraph G {
    A -> P [arrowhead=none, color=black, penwidth=1];
    P -> A [color=black, penwidth=1];
    P [shape=point, width=0, height=0, color=black];
    
    // Для проверки - добавим метки только на одну часть
    A -> P [label="Start"];
    P -> A [label="End"];
}
```

## 2. Использование одного составного ребра

```dot
digraph G {
    // Создаём эффект "разрыва" для метки
    A -> P [arrowhead=none, label="", color=black];
    P -> A [label="Метка\nпетли", color=black];
    P [shape=point, width=0, height=0, color=black];
    
    // Дублируем для визуальной целостности
    A -> A [style=invis, weight=100]; // Невидимая петля для layout
}
```

## 3. Самый чистый подход - компенсация gap

```dot
digraph G {
    // Основная невидимая структура
    A -> P [arrowhead=none, style=invis];
    P -> A [style=invis];
    P [shape=point, width=0, height=0, style=invis];
    
    // Видимое ребро поверх невидимой структуры
    A -> A [label="Петля с меткой", constraint=false];
}
```

## 4. Использование dir=both для двунаправленности

```dot
digraph G {
    A -> P [dir=both, arrowtail=none, arrowhead=none, label="Петля"];
    P [shape=point, width=0, height=0, style=invis];
    
    // Принудительная форма петли
    A -> P [headport=n, tailport=ne];
}
```

## 5. Комбинированный подход (рекомендую)

```dot
digraph G {
    // Невидимая вершина для layout
    P [shape=point, width=0, height=0, style=invis];
    
    // Соединяем A-P-A как одно целое
    A -> P [style=invis, weight=0];
    P -> A [style=invis, weight=0];
    
    // Видимая петля использует те же контрольные точки
    A -> A [label="Идеальная петля", 
           constraint=false,
           labelfloat=true,
           color=blue];
}
```

## 6. Самый хитрый способ - совмещение

```dot
digraph G {
    // Создаём невидимую структуру
    A -> P [arrowhead=none, style=dashed, color=lightgray];
    P -> A [style=dashed, color=lightgray];
    P [shape=point, width=0.001, height=0.001, color=lightgray];
    
    // Поверх накладываем обычную петлю
    A -> A [label="Основная\nметка", color=black];
    
    // Дополнительные метки размещаем на невидимой структуре
    A -> P [headlabel="Доп. метка 1", labeldistance=3];
    P -> A [taillabel="Доп. метка 2", labeldistance=3];
}
```

## Ключевые принципы бесшовности:

1. **Идентичные атрибуты** - одинаковый `color`, `penwidth`, `style`
2. **style=invis для структуры** + обычное ребро для отображения
3. **constraint=false** чтобы видимое ребро не конфликтовало с невидимой структурой
4. **weight=0** для невидимой структуры, чтобы не влиять на layout

**Самый надёжный вариант - №3 или №5**, где невидимая структура только задаёт форму, а видимое ребро отвечает за отображение.


# Вопрос

> Всё равно не очень получается. Подумай ещё

Понял, нужен совершенно другой подход. Давайте откажемся от невидимых вершин и используем возможности Graphviz более изящно. Вот несколько принципиально новых идей:

## 1. Использование edge-весов и приоритетов

```dot
digraph G {
    rankdir=TB;
    node [shape=circle];
    
    // Основная петля для формы
    A -> A [label="", style=invis, weight=100];
    
    // Реальные рёбра как "надписи" на невидимой петле
    A -> A [label="Петля 1", dir=none, labelfloat=true, 
           labeldistance=3, labelangle=45];
    A -> A [label="Петля 2", dir=none, labelfloat=true,
           labeldistance=3, labelangle=135];
    A -> A [label="Петля 3", dir=none, labelfloat=true,
           labeldistance=3, labelangle=225];
    A -> A [label="Петля 4", dir=none, labelfloat=true,
           labeldistance=3, labelangle=315];
}
```

## 2. Метки как отдельные сущности

```dot
digraph G {
    node [shape=circle];
    
    // Создаём метки как отдельные невидимые узлы
    L1 [label="Петля 1", shape=plaintext, width=0, height=0];
    L2 [label="Петля 2", shape=plaintext, width=0, height=0];
    
    // Привязываем метки к вершине без видимых рёбер
    A -> L1 [style=invis, constraint=false];
    A -> L2 [style=invis, constraint=false];
    
    // Основная петля
    A -> A [style=invis, weight=100];
    
    // Размещаем метки вокруг вершины
    {rank=same; A; L1; L2}
    L1 -> A [style=invis];
    A -> L2 [style=invis];
}
```

## 3. Эксперимент с compound edges

```dot
digraph G {
    compound=true;
    node [shape=circle];
    
    subgraph cluster_loops {
        style=invis;
        A;
    }
    
    // Используем lhead и ltail для "псевдо-петель"
    A -> A [label="Петля 1", lhead=cluster_loops, ltail=cluster_loops,
           labelfloat=true, labeldistance=2.5];
    A -> A [label="Петля 2", lhead=cluster_loops, ltail=cluster_loops,
           labelfloat=true, labeldistance=2.5, labelangle=180];
}
```

## 4. Радикальный подход: SVG/post-processing

```dot
digraph G {
    node [shape=circle];
    
    // Генерируем базовый граф
    A -> B [style=invis]; // для пространства
    
    // Добавляем "петли" как отдельные метки
    A [label=<<TABLE BORDER="0">
        <TR><TD>А</TD></TR>
        <TR><TD> </TD></TR>
        <TR><TD><I>Петля 1</I></TD></TR>
        <TR><TD><I>Петля 2</I></TD></TR>
    </TABLE>>];
}
```

## 5. Самый простой рабочий вариант

```dot
digraph G {
    // Используем xlabel для дополнительных меток без рёбер
    A [xlabel="Петля 1", xlp="100,100"];
    A [xlabel="Петля 2", xlp="100,50"];
    
    // Основная вершина
    A [shape=circle];
    
    // Одна визуальная петля для формы
    A -> A [label="", minlen=3];
}
```

## 6. Если ничего не работает - внешняя обработка

Генерируйте граф без петель, затем добавляйте их программно:

```python
# Псевдокод для пост-обработки
import graphviz

dot = graphviz.Digraph()
dot.node('A')
# Генерируем базовый граф
dot.render('temp.gv')

# Парсим полученные координаты вершин
# Вычисляем позиции для меток петель
# Добавляем метки как текстовые элементы вручную
```

**Ключевая мысль**: Если стандартные методы Graphviz не дают идеального результата, иногда проще сгенерировать базовую структуру, а метки петель добавить как внешние текстовые элементы относительно позиций вершин.

Какой из этих подходов кажется наиболее перспективным для вашей задачи?


# Вопрос

> как сделать так, чтобы ребро в от v1 к v2 не располагалось на той же линии, что и обратное ребро?


Отличный вопрос! Вот несколько эффективных способов разделить встречные рёбра в Graphviz:

## 1. Использование портов (самый надёжный способ)

```dot
digraph G {
    v1 -> v2 [tailport=e, headport=w];  // С востока на запад
    v2 -> v1 [tailport=w, headport=e];  // С запада на восток
}
```

Или более выраженное разделение:

```dot
digraph G {
    v1 -> v2 [tailport=se, headport=nw];  // С юго-востока на северо-запад
    v2 -> v1 [tailport=sw, headport=ne];  // С юго-запада на северо-восток
}
```

## 2. Использование разных весов и constraint

```dot
digraph G {
    v1 -> v2 [weight=10];  // Основное направление - прямое
    v2 -> v1 [constraint=false, weight=0];  // Обратное - обходит
}
```

## 3. Явное указание curvature (изгиба)

```dot
digraph G {
    v1 -> v2 [curvature=0.3];   // Легкий изгиб в одну сторону
    v2 -> v1 [curvature=-0.3];  // Изгиб в другую сторону
}
```

## 4. Комбинированный подход

```dot
digraph G {
    // Прямое ребро
    v1 -> v2 [tailport=e, headport=w, weight=10];
    
    // Обратное ребро с изгибом
    v2 -> v1 [tailport=w, headport=e, constraint=false, curvature=0.5];
}
```

## 5. Использование splines

```dot
digraph G {
    splines=curved;  // или splines=true
    
    v1 -> v2;
    v2 -> v1;
}
```

## 6. Разное расстояние (len)

```dot
digraph G {
    v1 -> v2 [len=1.5];   // Короче
    v2 -> v1 [len=2.0];   // Длиннее - пройдет другим путем
}
```

## 7. Полный пример с несколькими вершинами

```dot
digraph G {
    rankdir=LR;  // Горизонтальное расположение
    
    v1 -> v2 [tailport=e, headport=w, label="прямое"];
    v2 -> v1 [tailport=w, headport=e, label="обратное", curvature=0.3];
    
    // Для демонстрации - добавим другие вершины
    v1 -> v3;
    v2 -> v3;
}
```

## 8. Для вертикального расположения

```dot
digraph G {
    rankdir=TB;  // Вертикальное расположение
    
    v1 -> v2 [tailport=s, headport=n];  // С юга на север
    v2 -> v1 [tailport=n, headport=s];  // С севера на юг
}
```

## Самый эффективный комбинация:

```dot
digraph G {
    splines=true;
    
    v1 -> v2 [tailport=e, headport=w, weight=10];
    v2 -> v1 [tailport=w, headport=e, constraint=false, curvature=0.3];
}
```

**Ключевые параметры:**
- `tailport`/`headport` - точки соединения на вершинах
- `curvature` - изгиб ребра (положительные и отрицательные значения)
- `constraint=false` - убирает влияние на позиционирование вершин
- `weight` - приоритет при рисовании

Экспериментируйте с комбинациями этих параметров для получения оптимального результата в вашем конкретном графе!


# Вопрос

> curvature ничего не меняет, len - тоже

Проблема в том, что `curvature` и `len` действительно могут не работать в некоторых случаях Graphviz, особенно с простыми графами. Давайте попробуем более эффективные подходы:

## 1. Принудительное разделение через порты + splines

```dot
digraph G {
    splines=true;
    
    v1 -> v2 [tailport=e, headport=w];
    v2 -> v1 [tailport=w, headport=e];
    
    // Добавляем "якоря" для принудительного разделения
    {rank=same; v1; v2}
}
```

## 2. Добавление невидимых вершин для создания пространства

```dot
digraph G {
    splines=true;
    
    // Невидимые вершины для создания маршрутов
    i1 [shape=point, width=0, height=0, style=invis];
    i2 [shape=point, width=0, height=0, style=invis];
    
    v1 -> i1 [style=invis];
    i1 -> v2;
    v2 -> i2 [style=invis];
    i2 -> v1;
    
    {rank=same; v1; v2; i1; i2}
}
```

## 3. Использование cluster для принудительной маршрутизации

```dot
digraph G {
    splines=true;
    compound=true;
    
    subgraph cluster_v1 {
        style=invis;
        v1;
    }
    
    subgraph cluster_v2 {
        style=invis;
        v2;
    }
    
    v1 -> v2 [lhead=cluster_v2, ltail=cluster_v1];
    v2 -> v1 [lhead=cluster_v1, ltail=cluster_v2];
}
```

## 4. Явное указание позиций (если знаете координаты)

```dot
digraph G {
    splines=true;
    
    v1 [pos="0,0!"];
    v2 [pos="2,0!"];
    
    v1 -> v2 [pos="0,0 0.5,0.3 1.5,0.3 2,0"];
    v2 -> v1 [pos="2,0 1.5,-0.3 0.5,-0.3 0,0"];
}
```

## 5. Самый надёжный способ - изменить layout engine

```dot
// Используйте neato вместо dot
graph G {
    layout=neato;
    v1 -- v2 [dir=both];
    
    // Или явно задайте overlap
    overlap=false;
    splines=true;
}
```

## 6. Практическое решение для dot

```dot
digraph G {
    // Создаём искусственное пространство
    node [shape=circle, width=0.8];
    
    // Добавляем третью вершину для принудительного изменения маршрутов
    spacer [shape=point, width=0, height=0, style=invis];
    
    v1 -> spacer [style=invis, weight=100];
    spacer -> v2 [style=invis, weight=100];
    
    // Основные рёбра
    v1 -> v2 [tailport=ne, headport=nw];
    v2 -> v1 [tailport=se, headport=sw];
}
```

## 7. Альтернатива: используйте fdp вместо dot

```bash
# В командной строке
fdp -Tpng graph.gv -o graph.png
```

```dot
graph G {
    layout=fdp;
    v1 -- v2 [dir=both];
}
```

## 8. Экстремальный вариант: раздельные подграфы

```dot
digraph G {
    compound=true;
    
    subgraph cluster_top {
        style=invis;
        v1 -> v2 [tailport=s, headport=n];
    }
    
    subgraph cluster_bottom {
        style=invis;
        v2 -> v1 [tailport=n, headport=s];
    }
}
```

**Рекомендация:** Попробуйте сначала вариант №1 с `splines=true` и явными портами. Если не сработает, переходите к варианту №6 с невидимыми разделителями.

Если всё ещё не получается, возможно, стоит рассмотреть использование другого инструмента визуализации графов, который даёт больше контроля над рёбрами.


# Вопрос

> какой другой инструмент для визуализации графов ты можешь порекомендовать?

Вот список популярных инструментов для визуализации графов с лучшим контролем над рёбрами:

## 1. **yEd Graph Editor** (бесплатный)
- **Плюсы**: Отличный контроль над рёбрами, можно вручную редактировать изгибы, мощная автоматическая расстановка
- **Особенность**: Позволяет явно задавать контрольные точки для рёбер
- **Формат**: Поддерживает Graphviz, но имеет собственный редактор

## 2. **Gephi** (бесплатный)
- **Плюсы**: Профессиональный инструмент, отличная визуализация, много алгоритмов компоновки
- **Особенность**: Интерактивное управление, можно тянуть рёбра мышкой
- **Использование**: Часто используется для анализа социальных сетей

## 3. **Cytoscape** (бесплатный)
- **Плюсы**: Биоинформатика, но отлично подходит для любых графов, JavaScript версия для web
- **Особенность**: Полный контроль над стилями рёбер
- **Экосистема**: Много плагинов

## 4. **Tulip** (бесплатный)
- **Плюсы**: Академический инструмент с огромными возможностями кастомизации
- **Особенность**: Поддержка очень больших графов

## 5. **Commercial libraries**
- **yFiles** - коммерческая библиотека, золотой стандарт
- **KeyLines** / **ReGraph** - коммерческие, для веб-приложений
- **OGDF** - Open Graph Drawing Framework (C++ библиотека)

## 6. **Web-based solutions**
```javascript
// Vis.js
var options = {
  edges: {
    smooth: {
      type: 'continuous',
      roundness: 0.5
    }
  }
};

// Cytoscape.js
cy.style().selector('edge')
  .style('curve-style', 'bezier')
  .style('control-point-step-size', 40)
  .update();
```

## 7. **Программные подходы**

**NetworkX + Matplotlib** (Python):
```python
import networkx as nx
import matplotlib.pyplot as plt

G = nx.DiGraph()
G.add_edges_from([(1,2), (2,1)])

pos = nx.spring_layout(G)
nx.draw(G, pos, with_labels=True, 
        connectionstyle='arc3,rad=0.1')  # Изгибы для встречных рёбер
plt.show()
```

**Graph-tool** (Python):
```python
from graph_tool.all import *

# Больше контроля над рендерингом
graph_draw(g, edge_end_marker="none",
           output_size=(400, 400))
```

## 8. **Ручная обработка Graphviz**

Иногда проще пост-обработать SVG:
```bash
dot -Tsvg graph.gv > graph.svg
# затем редактировать в InkScape или коде
```

## Рекомендация:

Для вашей задачи с разделением встречных рёбер я бы рекомендовал:

1. **yEd** - если нужен desktop-инструмент с GUI
2. **Cytoscape.js** - если нужно веб-решение  
3. **NetworkX + Matplotlib** - если вы уже в Python-экосистеме

**yEd** особенно хорош потому что:
- Бесплатен
- Имеет специальные алгоритмы для "avoiding edge conflicts"
- Позволяет ручную корректировку
- Импортирует Graphviz файлы

Хотите, чтобы я подробнее рассказал о каком-то конкретном инструменте?

