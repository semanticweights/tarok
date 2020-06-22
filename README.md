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
<br/><br/>
<p align="center">
  <img src="https://upload.wikimedia.org/wikipedia/en/thumb/6/6c/TarockTrull.jpg/640px-TarockTrull.jpg" width="500" alt="Tarok trull cards"/>
</p>

[Slovenian Tarok](https://en.wikipedia.org/wiki/K%C3%B6nigrufen#Slovenia) is a variant of central European [Tarot card games](https://en.wikipedia.org/wiki/Tarot_card_games). It is essentially a three- or four-player, trick-taking, competitive game of skill with bidding. Computationally speaking, Tarok is moderately more complex than [Bridge](https://en.wikipedia.org/wiki/Contract_bridge) [[1]](#references). Detailed game rules are available [here](https://www.pagat.com/tarot/sltarok.html).

This repository provides a C++ game environment for the [OpenSpiel framework](https://github.com/deepmind/open_spiel) and custom Python bindings for `Game` and `State` objects [[2]](#references).

### Running in Docker
The easiest way to run Python scripts that use Tarok and OpenSpiel is via docker:
1. Run `git clone https://github.com/semanticweights/tarok.git` and `cd` into the repository
2. Run
```bash
docker run -v $(pwd)/tarok/python:/src -i --rm semanticweights/tarok:run-652e9b2 /src/play_game.py
```
Note that hash values after the *run-* part correspond to commits from the *stable* branch.

### Local Development
1. Run `git clone --recurse-submodules https://github.com/semanticweights/tarok.git` and `cd` into the repository
2. Ensure you have the following installed:
    - a compiler that supports the C++17 standard (tested with Clang)
    - `cmake` (version 3.13.4 or above)
    - `make`
    - `python3` (tested with 3.7.4)
3. Create a new virtual environment and activate it
4. Run `python3 -m pip install --upgrade pip setuptools`
5. Run `python3 -m pip install -r tarok/python/requirements-dev.txt`
6. Run `./tarok/install.sh`
7. Add Python modules to `PYTHONPATH` (see output from the previous step)

#### Running the Tests and Linter
- Run the tests with `./build/test/tarok_tests`
- Run the linter with `cpplint tarok/src/* tarok/test/*`

### References
- [1] [Luštrek Mitja, Matjaž Gams, Ivan Bratko. "A program for playing Tarok." ICGA journal 26.3 (2003): 190-197.](https://pdfs.semanticscholar.org/a920/70fe11f75f58c27ed907c4688747259cae15.pdf)
- [2] [Lanctot Marc et al. "Openspiel: A framework for reinforcement learning in games." arXiv preprint arXiv:1908.09453 (2019).](https://arxiv.org/pdf/1908.09453.pdf)
