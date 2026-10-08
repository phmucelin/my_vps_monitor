# My VPS Monitor

Monitor de uma VPS Linux escrito em C. O objetivo é detectar problemas no host e em contêineres Docker, enviar alertas, registrar quando o serviço se recuperar e, futuramente, produzir relatórios sobre o comportamento da VPS. Um verificador em outro servidor deverá avisar quando a própria VPS ficar inacessível.

Este é um projeto em desenvolvimento. **A versão atual apenas coleta e imprime CPU, memória e disco uma vez; ela ainda não monitora continuamente nem envia alertas.**

## Como a solução deve funcionar

```text
VPS monitorada
  └─ monitor em C → CPU, memória, disco, Docker → regras de alerta → email
         └─ heartbeat periódico para um servidor externo (planejado)

Servidor externo
  └─ verifica a chegada do heartbeat → alerta crítico se a VPS sumir
```

O verificador externo é necessário porque um programa rodando na VPS não consegue enviar um aviso se a máquina desligar ou perder completamente a rede. O heartbeat é a abordagem planejada: a VPS envia sinais periódicos ao outro servidor, sem precisar expor uma porta só para essa checagem. Também é possível verificar a VPS de fora com ping ou uma conexão HTTP/TCP; uma falha de ping isolada não comprova que a máquina desligou, pois pode haver bloqueio de ICMP ou falha de rede. O alerta deve dizer **“VPS inacessível”** até que a causa seja confirmada.

## Estado atual do código

| Área | Estado |
| --- | --- |
| CPU | Lê duas amostras de `/proc/stat` e calcula a utilização entre elas. |
| Memória | Lê `MemTotal` e `MemAvailable` de `/proc/meminfo` e calcula a utilização. |
| Disco | Usa `statvfs` para medir o sistema de arquivos montado em `/`. |
| Docker | Lista até 100 contêineres com `docker ps -a`, mas a função ainda não é chamada pelo programa principal. |
| Alertas, configuração e email | Arquivos preparados, ainda sem implementação. |
| Execução contínua | Ainda não existe: `main` faz uma coleta e termina. |

O código foi pensado para Linux por usar `/proc` e Docker. O executável `monitor` presente no repositório é um binário Linux; a instalação e o funcionamento na VPS ainda precisam ser validados.

## Próximos passos

### 1. Tornar a coleta confiável

- [ ] Corrigir a leitura de memória: inicializar os dados e confirmar que `MemTotal` e `MemAvailable` foram encontrados antes do cálculo.
- [ ] Fechar `/proc/stat` também quando a leitura da CPU falhar.
- [ ] Verificar o código de saída do comando Docker; erro de acesso ou daemon indisponível não pode parecer “zero contêineres”.
- [ ] Definir quais sistemas de arquivos monitorar e como tratar o espaço disponível para o usuário do serviço.

### 2. Rodar continuamente

- [ ] Criar um ciclo de coleta com intervalo configurável, por exemplo, 30 ou 60 segundos.
- [ ] Tratar sinais de encerramento e registrar erros sem encerrar por uma falha temporária de leitura.
- [ ] Executar como serviço `systemd`, com inicialização no boot, reinício após falha e logs consultáveis.
- [ ] Medir CPU e memória reais do processo na VPS após colocá-lo em execução.

### 3. Detectar incidentes locais

- [ ] Configurar limites de CPU, memória e disco e exigir leituras consecutivas ruins para evitar alertas por picos breves.
- [ ] Definir uma lista de contêineres que **devem** estar ativos; contêineres parados de propósito não são incidentes.
- [ ] Verificar estado `running` e, quando houver `HEALTHCHECK`, estado de saúde do contêiner.
- [ ] Registrar início, repetição controlada e recuperação de cada incidente, evitando um email a cada coleta.

### 4. Entregar alertas

- [ ] Implementar email e guardar credenciais fora do código e do repositório.
- [ ] Definir severidade, destinatários, tentativas de envio com limite e logs de falhas no envio.
- [ ] Testar a queda e a recuperação de um contêiner e a indisponibilidade do serviço de email.
- [ ] Avaliar Telegram ou WhatsApp como canais adicionais depois que o fluxo de email estiver estável.

### 5. Detectar a queda da VPS de fora

- [ ] Implementar o envio periódico de heartbeat pela VPS e a verificação em um servidor independente.
- [ ] Configurar um prazo para ausência de heartbeat e exigir falhas consecutivas antes do alerta crítico.
- [ ] Fazer **o servidor externo** enviar o alerta de indisponibilidade e depois o aviso de recuperação.
- [ ] Testar desligamento da VPS, perda de rede e falha do próprio monitor.

### 6. Histórico, relatórios e anomalias

- [ ] Guardar amostras com horário, métricas da VPS e identificação dos contêineres, com política de retenção para o histórico não crescer indefinidamente.
- [ ] Coletar também métricas por contêiner, como CPU e memória; a listagem atual do Docker informa estado, mas não mede o consumo de cada um.
- [ ] Gerar relatórios por período e horário do dia: média, máximo, duração dos picos e contêineres envolvidos.
- [ ] Detectar aumentos repentinos que logo desaparecem e comparar cada horário com o comportamento habitual daquele mesmo período.
- [ ] Relacionar picos a eventos, como reinício de contêiner, tarefa agendada ou falha, antes de classificá-los como anomalia.

Picos mais curtos que o intervalo de coleta podem passar despercebidos. A frequência das amostras deve ser escolhida conforme a duração dos eventos que queremos enxergar e o custo da coleta. Essa análise histórica é uma evolução do monitor; não é requisito para a primeira versão útil.

### 7. Jev, se acrescentar valor

- [ ] Experimentar a API do Jev para classificar ou priorizar incidentes ambíguos a partir de dados resumidos.
- [ ] Validar o JSON recebido e aceitar apenas categorias de resposta previamente definidas.
- [ ] Manter alertas objetivos, como “contêiner essencial parou”, funcionando mesmo sem acesso à API.
- [ ] Associar cada categoria a uma ação permitida no código; nunca executar comandos livres recebidos da API.

## Critério para considerar uma primeira versão útil

O monitor inicia automaticamente na VPS, identifica a parada de um contêiner essencial, envia **um** alerta, informa a recuperação e registra erros de coleta ou de envio. Em paralelo, o servidor externo avisa quando deixa de receber heartbeat. Esses cenários precisam ser testados de verdade antes de depender dos avisos.
