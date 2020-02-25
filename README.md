# Slovenian Tarok Card Game
<a href="https://circleci.com/gh/semanticweights/tarok">
  <img src="https://img.shields.io/circleci/build/github/semanticweights/tarok?style=flat-square" alt="build info"/>
</a>
<a href="https://hub.docker.com/repository/docker/semanticweights/tarok/tags">
  <img src="https://img.shields.io/docker/pulls/semanticweights/tarok?color=ff69b4&style=flat-square" alt="docker hub"/>
</a>
<a href="https://github.com/semanticweights/tarok/blob/master/LICENSE">
  <img src="https://img.shields.io/github/license/semanticweights/tarok?color=blue&style=flat-square" alt="license"/>
</a>

---

Slovenian Tarok card game environment for the [OpenSpiel framework](https://github.com/deepmind/open_spiel). For detailed game rules visit https://www.pagat.com/tarot/sltarok.html.

### Running in Docker
1. Clone this repository and cd into it
2. Run
```bash
docker run -v $(pwd)/tarok/python:/src -i --rm semanticweights/tarok:run-5dd69a1 python3 /src/main.py
```

### Local Development
1. Clone this repository and cd into it
2. Ensure you have the following installed:
    - **a compiler that supports the C++17 standard**
    - **cmake** (version **3.13.4** and above)
    - **make**
    - **python3** (tested with **3.7.4**)
3. Create a new virtual environment and activate it
4. Run `pip3 install --upgrade pip setuptools`
5. Run `pip3 install -r tarok/python/requirements.txt`
6. Run `./tarok/install.sh`
7. Add python modules to the python path (see output from the previous step)

#### Running the Tests and Examples
- Run the tests with `./build/test/tarok_tests`
- Run the [example python script](tarok/python/main.py) with `python tarok/python/main.py`

#### Running the Linter
- Run the linter with `cpplint tarok/src/* tarok/test/*`
