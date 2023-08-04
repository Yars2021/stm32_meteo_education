#ifndef _VAR_H_
#define _VAR_H_
     
#ifdef MAIN
// В одном из файлов проекта до всех include должен стоять дефайн MAIN
// Таким образом все эти переменные создаются именно в нем
// и именно тут можно задать начальную инициализацию

#else
// Для всех остальных файлов проекта эти переменные будут объявлены как extern.
// Ну а если дефайна MAIN не будет нигде то с ума сойдет линковщик
// А если несколько - то компилятор

#endif /* MAIN */
     
#endif /* _VAR_H_ */
