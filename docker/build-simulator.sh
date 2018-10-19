#!/bin/bash
docker-compose pull compiler
docker-compose build compiler
docker-compose pull simulator-compiler
docker-compose build simulator-compiler
docker-compose run --rm simulator-compiler
docker-compose build simulator
