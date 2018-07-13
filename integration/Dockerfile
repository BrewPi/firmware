FROM python:3.6

ENV SPARK_ADDRESS=localhost
ENV SPARK_PORT=5000

COPY . /app
WORKDIR /app

RUN pip3 install pipenv
RUN pipenv install --dev

ENTRYPOINT [ "pipenv", "run", "pytest" ]
