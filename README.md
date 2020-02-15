## Slovenian Tarok Card Game
<a href="https://circleci.com/gh/semanticweights/tarok">
  <img src="https://img.shields.io/circleci/build/github/semanticweights/tarok?style=flat-square" alt="build info"/>
</a>
<br/>

For rules see https://www.pagat.com/tarot/sltarok.html.

### Running in Docker
1. Clone this repository and cd into it
2. Run `docker run -v $(pwd)/tarok/python:/src -i --rm semanticweights/tarok:run-5dd69a1 python3 /src/main.py`

### Local Development
1. Clone this repository and cd into it
2. Ensure you have the following installed:
    - **c++ 17 compiler**
    - **cmake**
    - **make**
    - **python3** (tested with **3.7.4**)
3. Create a new virtual environment and activate it
4. Run `pip install --upgrade pip setuptools`
5. Run `pip install -r tarok/libs/open_spiel/requirements.txt`
6. Run `./tarok/install.sh`
7. Add python modules to the python path (see output from the previous step)

#### Running Tests and Examples
- Run the tests with `./build/test/tarok_tests`
- Run the [example python script](tarok/python/main.py) with `python tarok/python/main.py`

### Resources
- https://github.com/deepmind/open_spiel
