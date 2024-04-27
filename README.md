# Urban_Reconstruction

Библиотека позволяет строить текстурированную сетку (меш). Позволяет преобразовывать панорамные изображения с углом обзора 360° в формат cubemap, а также создает файлы поз камер, поддерживаемые библиотекой pcl, для каждого изображения формата cubemap. Позволяет создавать меш, а также применять фильтр, удаляющий достраиваемые методом Пуассона треугольники меша.

В библиотеке реализован алгоритм текстурирования, основанный на базе алгоритма текстурирования из библиотеки PCL. Алгоритм текстурирует исходя из принципа наибольшая площадь проекции, а затем можно применить оптимизацию переноса цвета между текстурами для одного и того же полигона, чтобы учитывать цвет с нескольких кадров. 

Были произведены замеры метрик для 1 – изображения, 2 – алгоритм выбора текстуры с наибольшей площадью проекции, 3 – алгоритм выбора текстуры с наибольшей площадью проекции + перенос цвета, 4 --- алгоритм текстурирования из библиотеки PCL, 5 – алгоритм текстурирования из библиотеки PCL + перенос цвета.

<img src="img/lidar6_metrics_full_presa.jpg" alt="Сравнение мешей">
<img src="img/lidar6_metrics_presa.jpg" alt="Сравнение мешей">
<img src="img/lidar10_metrics_full_presa.jpg" alt="Сравнение мешей">
<img src="img/lidar10_metrics_presa.jpg" alt="Сравнение мешей">
<img src="img/lidar15_metrics_full_presa.jpg" alt="Сравнение мешей">
<img src="img/lidar15_metrics_presa.jpg" alt="Сравнение мешей">
