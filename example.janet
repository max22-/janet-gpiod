(import ./build/gpiod)
(def chip (gpiod/chip-open "gpiochip0"))
(def line (gpiod/get-line chip 4))
(gpiod/request-output line)
(gpiod/set-value line 0)
(ev/sleep 1)
(gpiod/set-value line 1)
(ev/sleep 1)
(gpiod/set-value line 0)
(gpiod/line-release line)
(gpiod/chip-close chip)
