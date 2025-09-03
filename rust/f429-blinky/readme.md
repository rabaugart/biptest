# Rust-Embassy Versuche auf F429-Discovery Board

## 3.9.2025 Blinky-Beispiel

Grundlage ist das embassy Beispiel [blinky](https://github.com/embassy-rs/embassy/blob/main/examples/stm32f4/src/bin/blinky.rs).
Nur die LED wird noch auf `PG13` gesetzt.

Die Datei `memory.x` wird irgendwo im `target`-Verzeichnis erzeugt und muss von
Hand ins Toplevel-Verzeichnis kopiert werden.

Das Flashen mit `probe-rs` funktioniert nicht. Bisherige Lösung: wandle
das ELF-Executable mit den [`cargo-binutils`](https://github.com/rust-embedded/cargo-binutils) in das `bin`-Format um
und kopiere die Datei wie üblich über USB.
Umwandlungt mit:

    cargo objcopy --release -- -O binary app.bin

Installieren der Binutils mittels

    cargo install cargo-binutils

Dast Bin-File liegt unter `binary-artefacts/blinky.bin`.

Das Board startet die Applikation bisher nur wenn es an einem aktiven USB-HOST hängt.
Die rote LED `LD2` blinkt, falls nicht. Das deutet auf ein Power-Problem hin.
