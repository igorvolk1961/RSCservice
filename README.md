# RSCService

HTTP-сервис для работы с RSC-данными: чтение сегментов и объектов, семантика, построение SVG-изображений. Реализован на Qt и [QtWebApp](https://github.com/stefanfrings/QtWebApp).

## Возможности

- **RSC API** (`/rsc/*`) — JSON API для:
  - создания SVG-билдера по файлу RSC;
  - получения incode по excode и по ключу;
  - работы с сегментами и объектами RSC (количество, данные);
  - семантических классов и простой семантики;
  - генерации SVG-изображений.
- **Отладочный endpoint** `/dump` — вывод параметров и тела HTTP-запроса в HTML.
- Раздача статических файлов из настроенной корневой директории (docroot).

## Требования

- **Qt 5** (модули: core, network, xml, gui)
- **QtWebApp** — клонировать в каталог рядом с проектом: `../QtWebApp`
- **libcurl** (для зависимостей)
- **rsccomm** — исходники входят в репозиторий (`iml/rsccomm`, зависимость от `iml/imlcomm`), библиотека собирается в `./lib`
- **GISCore** — в `.pro` задаётся один из вариантов:
  - `GIS = GIS_OPERATOR` — заголовки в `/usr/local/GISCore-operator12/include`
  - `GIS = GIS_INTEGRATION` — заголовки в `/usr/local/GISCore-9.20.0/include`

## Сборка

```bash
# Клонировать QtWebApp рядом с RSCService (если ещё не клонирован)
git clone https://github.com/stefanfrings/QtWebApp.git ../QtWebApp

# 1) Собрать библиотеку rsccomm (исходники в iml/rsccomm)
cd iml/rsccomm
qmake rsccomm.pro
make          # Linux/macOS — создаётся lib/librsccomm.so*
# или nmake / mingw32-make на Windows — lib/rsccomm.lib или lib/librsccomm.a
cd ../..

# 2) Собрать RSCService
qmake RSCService.pro
make          # Linux/macOS
# или
nmake         # Windows (MSVC)
# или
mingw32-make  # Windows (MinGW)
```

Исполняемый файл создаётся в `./bin/RSCService` (или `RSCService.exe` на Windows).

## Конфигурация

Приложение ищет файл **RSCService.ini** в каталоге запуска, в `etc`, в родительских каталогах или в `/etc/QtWebAppGis`, `/etc/opt`. Пример структуры:

```ini
[listener]
port=8080
minThreads=1
maxThreads=100

[logging]
minLevel=0
bufferSize=100
maxSize=10000

[docroot]
path=./data/docroot
encoding=UTF-8

[service]
nofile=2048
```

Создайте `RSCService.ini` (например, в `bin/` или в `etc/`) перед первым запуском.

## Запуск

Из каталога, где лежит `RSCService.ini` (или где приложение его находит):

```bash
./bin/RSCService   # Linux/macOS
bin\RSCService.exe # Windows
```

В логах должно появиться сообщение о старте приложения.

## API (RSC)

Все запросы к `/rsc/*` — **POST**, тело — **JSON**, ответ — **application/json**.

| Путь | Назначение |
|------|------------|
| `/rsc/createSvgBuilder` | Создание SVG-билдера (файл RSC, масштаб и т.д.) |
| `/rsc/getIncodeForExcode` | Incode по excode |
| `/rsc/getIncodeForKey` | Incode по ключу |
| `/rsc/getRscSegmentCount` | Количество сегментов |
| `/rsc/getRscObjectCount` | Количество объектов |
| `/rsc/getRscSegment` | Данные сегмента |
| `/rsc/getRscObject` | Данные объекта |
| `/rsc/getRscSegmentLocalCount` | Локальное количество в сегменте |
| `/rsc/getRscSegmentLocalIncodes` | Локальные incode в сегменте |
| `/rsc/getRscSemanticClassCount` | Количество семантических классов |
| `/rsc/getRscSemanticClass` | Данные семантического класса |
| `/rsc/getRscSimpleSemanticCount` | Количество записей простой семантики |
| `/rsc/getRscSimpleSemantic` | Данные простой семантики |
| `/rsc/getSvgImage` | Получение SVG-изображения |

При ошибке разбора JSON или неизвестном пути в ответе возвращается JSON с полем `"error"`.

## Структура проекта

```
RSCService/
├── RSCService.pro      # проект qmake
├── src/
│   ├── main.cpp
│   ├── requestmapper.cpp/h   # маршрутизация HTTP
│   ├── controller/
│   │   ├── dumpcontroller.*  # /dump
│   │   └── rsccontroller.*   # /rsc/*
│   ├── common/               # общие утилиты и исключения
│   └── include/
├── iml/
│   ├── rsccomm/              # библиотека RSC (исходники в репозитории)
│   │   └── rsccomm.pro
│   └── imlcomm/              # общие модули для rsccomm
├── lib/                      # сюда собирается librsccomm (после сборки iml/rsccomm)
├── test/                     # тесты
├── data/
│   └── bin/                  # скрипты и unit-файлы (SXFService)
└── README.md
```

Внешние зависимости (вне репозитория):

- `../QtWebApp` — логирование и HTTP-сервер
- GISCore — заголовки и линковка по инструкциям поставщика


