# instructions
The current routine is a circular color recognition routine (red, green, blue) that can be tested using the following image

![](red.png "first")

![](green.png "second")

![](blue.png "third")

The successful recognition results are as follows

`recognize color is red!`

or

`recognize color is green!`

or

`recognize color is blue!`

Recognition failure is

`Not recognized!`

If you are using 0v7725, uncomment the following two lines of code

``` c
sensor.set_hmirror(True)
sensor.set_vflip(True)
```

Different environments have different thresholds. Please adjust them according to the thresholds of your environment, namely the following three thresholds: red, green and blue

``` c
red_threshold = (53, 31, 44, 82, 18, 78)
green_threshold = (54, 90, -64, -30, -19, 108)
blue_threshold = (34, 65, 22, 60, -100, -29)
```

The corresponding operation of the IDE is: tool -> machine vision -> threshold editor -> frame buffer
