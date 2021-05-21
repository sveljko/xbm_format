# xbm_format

Simple "single header" C library for working with the XBM monochrome bitmap format.
More precisely, only the "XBM 11" (newer) version of the format.

This format originated in X Window server and is rarely used these days, but, 
it is useful when working with monochrome displays, which are still often 
encountered in embedded systems.

Library is designed to be combined easily with the [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)
library.

The documentation is in the [file itself](xbm_format.h). 

The license is Public Domain or MIT, whichever you choose. MIT is offered because Public Domain is
not really supported in all jurisdictions across the world.

The library is designed to be simple to maintain and understand, there are no "tricks for speed".
If you do find it slow for a purpose, let me know, there are things that can be done to make it
work faster.

There is a simple [unit test](xbm_format.t.c).

## А сад на српском

Ово је "једнозаглавна" Ц библиотека за рад са XBM форматом за монохроматске слике.
Овај формат није популаран већ дуже времена, али је користан за рад са монохроматски
дисплејима који се често срећу у угњежденим системима.

Билбиотека је предвиђена за лаку употребу у сарадњи са  [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h).

Документација (додуше, само на енглеском) је у [самој датотеци](xbm_format.h). 
Требало би да је све прилично јасно и "уобичајено", па нисам ни правио превод
на српски.

Дозвола/лиценца је МИТ или "јавно добро", по Вашем избору, при чему је МИТ понуђен
јер "јавно добро" није баш јасно дефинисано у многим правосуђима по свету (па
ни српском, ако ћемо право).

Библиотека је осмишљена да буде јасна и лака за одржавање, а не брза. Ако се испостави
да за Вашу потребу ипак није довољно брза, можемо да порадимо на томе да је убрзамо.

Постоје и минимални али илустративан [тест модула](xbm_format.t.c).
