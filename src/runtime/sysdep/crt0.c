#include <stdlib.h>

// Объявляем глобальные переменные (стандартные для окружения Apple/BSD)
int NXArgc;
char **NXArgv;
char **environ;
char *__progname;

// Объявляем main, чтобы компилятор знал о нем
extern int main(int argc, char **argv, char **envp);

/*
 * Вспомогательная функция на C.
 * Сюда мы попадаем из asm. Тут проще и нагляднее заполнить переменные.
 */
void _c_startup(int argc, char **argv, char **envp) {
    NXArgc = argc;
    NXArgv = argv;
    environ = envp;
    
    // Обычно __progname — это argv[0] (имя запущенного файла)
    __progname = (argv && argv[0]) ? argv[0] : "";

    // Запускаем main и передаем результат в exit
    exit(main(argc, argv, envp));
}

/*
 * Точка входа (Entry Point).
 * Задача: достать аргументы со стека, выровнять стек и вызвать C-код.
 */
__asm__(
    ".text\n"
    ".arm\n"
    ".align 2\n"
    ".globl start\n"
    "start:\n"
    // Стек при старте: [argc, argv[0], ..., NULL, envp[0], ...]
    
    "    ldr r0, [sp]            @ r0 = argc\n"
    "    add r1, sp, #4          @ r1 = argv (адрес начала массива аргументов)\n"
    
    // Вычисляем адрес envp: argv + (argc + 1) * 4
    "    add r2, r0, #1          @ r2 = argc + 1\n"
    "    add r2, r1, r2, lsl #2  @ r2 = r1 + (r2 * 4) -> теперь в r2 лежит envp\n"
    
    "    bic sp, sp, #7          @ Выравниваем стек по 8 байт (требование ARM ABI)\n"
    
    // Вызываем нашу C-функцию. 
    // На Mac/iOS компилятор добавляет нижнее подчеркивание к именам C-функций.
    "    bl __c_startup\n" 
    
    "    trap                    @ Сюда попасть не должны, так как exit не возвращает управление\n"
);
