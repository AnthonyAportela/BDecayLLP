from scailfin/madgraph5-amc-nlo:mg5_amc3.4.2

ENV PYTHIA8DATA /usr/local/venv/share/Pythia8/xmldoc
ENV PYTHIA_VERSION 8306
ENV HEPMC_DIR /usr/local/venv
ADD  . /code
RUN cd /code && \
    cmake -S . -B build && \
    cmake --build build -- && \
    cmake --build build --target install && \
    rm -rf /code