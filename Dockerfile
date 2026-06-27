FROM ubuntu:22.04

LABEL description="BOS - Basic Operating System build environment"

# Evite les prompts interactifs
ENV DEBIAN_FRONTEND=noninteractive

# Installe les outils de compilation
RUN apt-get update && apt-get install -y \
    nasm \
    gcc \
    gcc-multilib \
    binutils \
    make \
    git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /bos

# Copie les sources
COPY . .

# Compile et nettoie les fichiers intermédiaires (garde uniquement bos.img)
RUN make dist

# Par défaut : affiche l'image produite
CMD ["ls", "-lh", "bos.img"]
