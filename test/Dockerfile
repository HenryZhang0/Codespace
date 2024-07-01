FROM python:3.7.17
WORKDIR /app
COPY . /app
# RUN python3.7 -m pip install --upgrade pip setuptools wheel \ 
RUN python -m pip install --upgrade pip \ 
    && python -m pip install flake8 pytest wheel numpy==1.19.2 \
    && python -m pip install -r requirements.txt
EXPOSE 3000
CMD python3.7 ./index.py/
