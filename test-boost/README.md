

Notizen

 * Probleme auf Mac: [Erklärung](https://stackoverflow.com/questions/35305291/boost-interprocess-condition-multiple-threads-calling-wait-fails)
 * [String](https://stackoverflow.com/questions/4278627/c-boostinterprocess-simple-application), der für die Benanumg verwendet werden soll.
 * POSIX [threads](https://computing.llnl.gov/tutorials/pthreads/)
 * [Sharable](https://stackoverflow.com/questions/20325146/share-condition-variable-mutex-between-processes-does-mutex-have-to-locked-be) mutex und [hier](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_condattr_setpshared.html)
   [opengroup](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutexattr_getpshared.html#), [Erläuterungen](http://pubs.opengroup.org/onlinepubs/9699919799/functions/V2_chap02.html#tag_15_09_09)
 * Windows [Vergleich](https://stackoverflow.com/questions/5233681/windows-condition-variable-vs-event#5234924) Condition Variables und Events.
   msdn-[Beispiel](https://msdn.microsoft.com/en-us/library/windows/desktop/ms686903(v=vs.85).aspx)
   für Condition-Variables in Threads.
   [Übersicht](http://www.cs.wustl.edu/~schmidt/win32-cv-1.html) Win32-Condition-Möglichkeiten.
   [Variante](https://stackoverflow.com/questions/6895758/inter-process-condition-variables-in-windows) mit Semaphoren zur   Emulation von Conditions zwischen Prozessen.
