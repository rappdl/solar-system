# Solar System Console App

This is a simple console application built in C that shows some information about the solar system. 

The app uses the public [Solar System openData API](https://api.le-systeme-solaire.net/).

## Installation
First you need to install the [CURL](https://curl.se/libcurl/) and [JSON](https://jansson.readthedocs.io/en/2.7/index.html) libraries.
The CURL library is used to stablish the connections to the server using HTTP and the JSON library is used to interpret the response from the server.

Run these commands in the terminal to install the libraries:
```bash
    $ sudo apt-get install libcurl4-gnutls-dev
    $ sudo apt-get install libjansson-dev
```



## Application usage

1. The first step is to run the application is run the makefile.
2. The second step is to run the main executable file.

This menu will be printed.

    Options:
    1. List all bodies
    2. List planets
    3. List satellites
    4. List one body of your choice
    5. Exit
    Insert command: (?)


### Actions
```
1. Will print  print all the bodies present in solar system.
2. Will print the planets present in solar system.
3. Will print the satellites of the solar system.
4. Will print ask you to introduce a body of your choice and will print all the information of that body.
5.  Will exit the application.
```

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.


## License
MIT License

Copyright (c) 2020 Solar System Console App

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
