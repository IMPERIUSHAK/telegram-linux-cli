# linux-telegram-cli

🚀 **linux-telegram-cli** — это Telegram-клиент для Linux, работающий полностью через командную строку (CLI), построенный на базе библиотеки [TDLib (Telegram Database Library)](https://github.com/tdlib/td).

## 📦 Возможности

- Авторизация по номеру телефона
- Поддержка отправки и получения сообщений
- Работа с TDLib напрямую
- Поддержка различных состояний авторизации
- Потенциал для расширения: боты, чаты, контакты и т.д.

## 🛠️ Установка

Перед сборкой убедитесь, что установлены все зависимости TDLib.

```bash
sudo apt install g++ cmake git openssl zlib1g-dev
git clone https://github.com/IMPERIUSHAK/linux-telegram-cli.git
cd linux-telegram-cli
mkdir build && cd build
cmake ..
make
```
## 🚀 Запуск
После установки запустите make файл

```
 mkdir build && cd build

cmake -DCMAKE_BUILD_TYPE=Release \
      -DTd_DIR=~/Projects/linux-cli/td/local/lib/cmake/Td \
      ..

make
./linux-cli
```

##  🔧 Зависимости

   * TDLib

   * C++17

   * CMake
