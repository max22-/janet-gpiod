(declare-project :name "gpiod")
(declare-native
 :name "gpiod"
 :source @["janet-gpiod.c"]
 :lflags [;default-lflags "-lgpiod"])
