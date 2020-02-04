## Slovenian Tarok Card Game
For rules see https://www.pagat.com/tarot/sltarok.html.

### Installation
1. Clone this repository and cd into it
2. Ensure you have the following installed:
    - **c++ 17 compiler**
    - **cmake**
    - **make**
    - **python3.6**
3. Create a new virtual environment and activate it
4. Run `pip install -r tarok/libs/open_spiel/requirements.txt`
6. Run `./tarok/install.sh`
7. Add pyspiel to the python path (see output from the previous step)

### Usage
- Run the tarok binary with `./build/tarok`
- Run the python script with `python3 tarok/python/main.py`

### Resources
- https://github.com/deepmind/open_spiel
