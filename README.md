# Configuração dos softwares necessários
É necessária a instalação de:
* [BlueZ][1] - Implementação do stack de protocolo Bluetooth para Linux
* [WiiUse][2] - API para conexão com o wiimote
* [arduino-serial][3] - API para comunicação com o arduino via portas seriais
* [Adafruit Motor Shield library][6] - Biblioteca para o shield motor do arduino

### Pareando o bluetooth com o computador
Durante a execução deste projeto, o sensor bluetooth só pode ser conectado após a remoção do pacote **brltty**, utilizado para a funcionalidade de leitura de tela (display "braile"), [post sobre o assunto][6].

Primeiro verifique a existência de dispositivos pareados procurando por arquivos de dispositivos no diretório ``/dev`` utilizando
```bash
ls /dev | grep 'rfcomm'
```

ou apenas digitando ``ls /dev/rf``, pressionando ``TAB`` algumas vezes e observar o resultado do auto-complete.

Para fazer o 'bind' do device, primeiramente encontre o endereço do dispositivo a partir do *terminal* fazendo

```bash
$ hcitool scan
```

Caso encontrado o dispositivo de nome ``HC-05``, copie o endereço e execute o seguinte comando:

```bash
$ sudo rfcomm bind <numero_do_dispositivo> <endereco_do_dispositivo>
```

Supondo que nenhum arquivo de dispositivo *rfcomm\** tenha sido encontrado em ``/dev``

```bash
$ sudo rfcomm bind 0 <endereco_do_dispositivo>
```

Caso este dispositivo tenha sido configurado, ele deverá aparecer ao se executar
```bash
ls /dev | grep 'rfcomm'
```

[1]: http://www.bluez.org
[2]: https://github.com/rpavlik/wiiuse
[3]: https://github.com/todbot/arduino-serial/
[4]: http://packages.ubuntu.com/trusty/synaptic
[5]: http://public.vrac.iastate.edu/vancegroup/docs/wiiuse/d4/d92/group__publicapi.html "WiiUse External API"
[6]: http://playground.arduino.cc/Learning/ArduinoBT-Ubuntu
[7]: https://github.com/adafruit/Adafruit-Motor-Shield-library
