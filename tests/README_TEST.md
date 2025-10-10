# راهنمای اجرای تست‌های پروژه Airline System

## پیش‌نیازها
- نصب g++ (MinGW یا MSYS2)
- نصب Catch2 (در این پروژه فایل catch_amalgamated.hpp و catch_amalgamated.cpp موجود است)

## کامپایل تست‌ها
برای کامپایل تست‌ها باید همه فایل‌های مورد نیاز را با هم کامپایل کنید:

```sh
C:\msys64\ucrt64\bin\g++.exe -fdiagnostics-color=always -g tests/airline_tests.cpp tests/catch_amalgamated.cpp main/AirlineSystem.cpp main/Passenger.cpp main/Flight.cpp main/Reservation.cpp main/FileManager.cpp main/InputValidator.cpp main/main.cpp -o tests/airline_tests.exe
```

> توجه: اگر در ویندوز هستید، مسیرها را با \ بنویسید یا از PowerShell همین دستور را اجرا کنید.

## اجرای تست‌ها
بعد از کامپایل، برای اجرای تست‌ها کافی است دستور زیر را اجرا کنید:

```sh
./tests/airline_tests.exe
```
یا در ویندوز:
```sh
.\tests\airline_tests.exe
```

## نکات
- خروجی تست‌ها و فایل‌های گزارش در همان پوشه پروژه ایجاد می‌شوند.
- برای اضافه کردن تست جدید، کافی است به فایل `tests/airline_tests.cpp` بخش جدید اضافه کنید.
- اگر با خطای لینک مواجه شدید، مطمئن شوید همه فایل‌های cpp پروژه و catch_amalgamated.cpp را در کامپایل قرار داده‌اید.
