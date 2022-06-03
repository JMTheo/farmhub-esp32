# FarmHub Arduino

## Instalação

É necessário ter o [Arduino](https://www.arduino.cc/en/software) instalado, as bibliotecas [ESPMQTTClient](https://github.com/plapointe6/EspMQTTClient), [DHT11](https://github.com/adidax/dht11), [ArduinoJSON](https://arduinojson.org/) e as configurações de placa do ESP32 da Espressif

Para instalar as placas é necessário abrir o Arduino IDE > Arquivo > Preferências, e acrescentar a url do ESP32

```bash
https://dl.espressif.com/dl/package_esp32_index.json
```

Feito isso, só adicionar a placa pelo gerenciador de placas, vale ressaltar que as demais bibliotecas devem ser instaladas pelo Arduino IDE

## Executar o projeto

Selecione a placa e a porta COM, essa versão incluir OTA. Então após o primeiro update os demais podem ser via OTA
