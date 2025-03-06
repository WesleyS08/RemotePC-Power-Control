# RemotePC-Power-Control

O **RemotePC-Power-Control** é um projeto que utiliza um **ESP32** para controlar remotamente o acionamento de um PC via **Wake-on-LAN (WOL)** e **MQTT**. Com esse sistema, é possível enviar comandos para ligar o PC remotamente a partir de qualquer dispositivo com acesso ao servidor MQTT configurado.

## Funcionalidades
- Conexão segura via **Wi-Fi**.
- Comunicação com servidor **MQTT** utilizando **TLS/SSL**.
- Envio de pacotes **Wake-on-LAN** para iniciar o PC remotamente.
- Controle remoto através de mensagens MQTT.

## Tecnologias Utilizadas

| Tecnologia           | Descrição                                                        |
|----------------------|------------------------------------------------------------------|
| **ESP32**            | Microcontrolador que gerencia a conexão Wi-Fi e envio de pacotes WOL. |
| **MQTT**             | Protocolo de mensagens para comunicação entre dispositivos.      |
| **WiFiClientSecure** | Biblioteca para conexão segura ao servidor MQTT.                |
| **Wake-on-LAN (WOL)**| Protocolo para enviar pacotes que acordam o PC remotamente.      |

## Como Funciona

1. O **ESP32** se conecta à rede Wi-Fi e ao servidor **MQTT**.
2. Ao receber uma mensagem MQTT com o comando `"ligarPC"`, o **ESP32** envia um pacote **Wake-on-LAN** para o PC configurado.
3. O PC, ao receber o pacote, é ativado remotamente, permitindo seu uso sem a necessidade de interação física.

## Como Configurar

1. **Configuração Wi-Fi:**
   - Defina as credenciais da sua rede Wi-Fi no código:
   
     ```cpp
     const char* ssid = "NOME_DA_REDE_WIFI";
     const char* password = "SENHA_DA_REDE";
     ```

2. **Configuração MQTT:**
   - Defina o servidor, a porta, o usuário e a senha do MQTT:
   
     ```cpp
     const char* mqtt_server = "SEU_SERVIDOR_MQTT";
     const int mqtt_port = 8883;
     const char* mqtt_user = "USUARIO_MQTT";
     const char* mqtt_password = "SENHA_MQTT";
     ```

3. **Configuração Wake-on-LAN:**
   - Informe o endereço **MAC** do PC e a porta de WOL:
   
     ```cpp
     const char* pc_mac_str = "XX:XX:XX:XX:XX:XX"; 
     uint8_t mac_address[6];
     const int wol_port = 9;
     ```

4. **Compilar e carregar no ESP32:**
   - Compile o código e faça o upload para o ESP32.

5. **Testar o Controle:**
   - Envie a mensagem MQTT com o comando `"ligarPC"` para o tópico `comando/wol` para acionar o PC remotamente.

## Como Obter o Endereço MAC e Ativar o Wake-on-LAN no PC

### 1. Obter o Endereço MAC do PC

O **endereço MAC** (Media Access Control) é necessário para enviar o pacote Wake-on-LAN ao PC. Aqui estão os passos para encontrá-lo, dependendo do sistema operacional:

#### **Windows:**
1. Pressione `Win + R` para abrir o diálogo "Executar".
2. Digite `cmd` e pressione Enter para abrir o Prompt de Comando.
3. No prompt de comando, digite o seguinte comando e pressione Enter:
   ```cmd
   ipconfig /all
4. Localize a seção Adaptador de Rede Ethernet ou Adaptador de Rede Sem Fio.
5. Procure pelo campo Endereço Físico. Esse é o seu endereço MAC.
6. 
#### **Linux:**
1. Abra o terminal.
Digite o seguinte comando e pressione Enter:
bash
Copiar
Editar
ifconfig
Localize a interface de rede ativa (geralmente eth0 para conexões com fio ou wlan0 para conexões sem fio).
O endereço MAC será listado após o campo ether.


2. Ativar o Wake-on-LAN na Placa de Rede
Para que o seu PC seja capaz de ser ligado remotamente via Wake-on-LAN, você precisa garantir que o recurso Wake-on-LAN (WOL) esteja habilitado na sua placa de rede. Aqui estão os passos para ativar essa funcionalidade:

Windows:
Abra o Gerenciador de Dispositivos (pressione Win + X e selecione "Gerenciador de Dispositivos").
Expanda a seção Adaptadores de Rede.
Clique com o botão direito no adaptador de rede que você está utilizando e selecione Propriedades.
Vá para a aba Avançado e procure por uma opção chamada Wake-on-LAN ou Power Management.
Selecione Wake-on-LAN ou Ativar no campo correspondente.
Na aba Gerenciamento de Energia, certifique-se de que a opção Permitir que este dispositivo ative o computador esteja marcada.
Linux:
Abra o terminal.
Verifique se o WOL está ativado com o comando:
bash
Copiar
Editar
ethtool eth0
(Substitua eth0 pela sua interface de rede.)
Se o WOL estiver desabilitado, use o comando a seguir para ativá-lo:
bash
Copiar
Editar
sudo ethtool -s eth0 wol g
Isso habilita o WOL via "magic packet".
macOS:
Acesse as Preferências do Sistema > Economia de Energia.
Certifique-se de que a opção "Ativar o modo de repouso de rede" está marcada.
Reinicie o sistema para aplicar as configurações.
3. Testar o Wake-on-LAN
Após configurar o endereço MAC e habilitar o Wake-on-LAN, você pode testar se o seu PC está pronto para ser ativado remotamente. No projeto RemotePC-Power-Control, você pode enviar o comando MQTT "ligarPC" para o tópico comando/wol, e o ESP32 enviará um pacote Wake-on-LAN para o endereço MAC configurado.

Se tudo estiver configurado corretamente, o PC será ligado remotamente quando o comando for recebido.

css
Copiar
Editar

## Exemplos de Comandos MQTT

| Tópico         | Comando   | Descrição                    |
|----------------|-----------|------------------------------|
| `comando/wol`  | `ligarPC` | Liga o PC via Wake-on-LAN.   |

## Licença

Este projeto é licenciado sob a [MIT License](LICENSE).
