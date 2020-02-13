## Slovenian Tarok Card Game
![CircleCI](https://img.shields.io/circleci/build/github/semanticweights/tarok?style=flat-square)

For rules see https://www.pagat.com/tarot/sltarok.html.

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

### Usage
- Run the [example python script](tarok/python/main.py) with `python tarok/python/main.py`
- Run the tests with `./build/test/tarok_tests`

### Resources
- https://github.com/deepmind/open_spiel
