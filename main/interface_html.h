#pragma once


const char interface_html [] = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Monitoramento da Qualidade da Água</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            margin: 0;
            background-color: #f0f0f0;
        }

        .container {
            text-align: center;
            background: white;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
        }

        .flag {
            margin: 20px;
            padding: 20px;
            border: 2px solid #ccc;
            border-radius: 8px;
        }

        #phFlag {
            border-color: #4CAF50; /* Verde */
        }

        #tempFlag {
            border-color: #FF9800; /* Laranja */
        }

        #turbidityFlag {
            border-color: #2196F3; /* Azul */
        }

        h2 {
            margin: 0 0 10px 0;
        }

        p {
            margin: 0;
            font-size: 1.2em;
        }

        .alert {
            color: #FF5722; /* Deep Orange for alert text */
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Monitoramento da Qualidade da Água</h1>
        <div class="flag" id="phFlag">
            <h2>pH da Água</h2>
            <p id="phValue">pH: --</p>
        </div>
        <div class="flag" id="tempFlag">
            <h2>Temperatura</h2>
            <p id="tempValue">Temperatura: -- °C</p>
        </div>
        <div class="flag" id="turbidityFlag">
            <h2>Turbidez</h2>
            <p id="turbidityValue">Turbidez: -- NTU</p>
        </div>
    </div>
    <script>
        // Função para atualizar os valores de monitoramento
        function updateMonitor(ph, temperature, turbidity) {
            updateValue('phValue', ph, 'pH');
            updateValue('tempValue', temperature, 'Temperatura', '°C');
            updateValue('turbidityValue', turbidity, 'Turbidez', 'NTU');

            updateFlagColor('phFlag', ph, 6.5, 8.5);
            updateFlagColor('tempFlag', temperature, 0, 35);
            updateFlagColor('turbidityFlag', turbidity, 0, 5);
        }

        // Função para atualizar o valor e unidade
        function updateValue(elementId, value, label, unit = '') {
            document.getElementById(elementId).innerText = `${label}: ${value} ${unit}`;
        }

        // Função para atualizar a cor da flag
        function updateFlagColor(flagId, value, minValue, maxValue) {
            const flag = document.getElementById(flagId);
            if (value < minValue || value > maxValue) {
                flag.style.backgroundColor = '#FFCDD2';  // Vermelho claro para valores fora do ideal
                flag.querySelector('p').classList.add('alert');
            } else {
                flag.style.backgroundColor = '#C8E6C9';  // Verde claro para valores dentro do ideal
                flag.querySelector('p').classList.remove('alert');
            }
        }

        // Função para obter dados do endpoint /data
        async function fetchData() {
            try {
                const response = await fetch('/data');
                if (!response.ok) {
                    throw new Error('Erro na requisição'); 
                }
                const data = await response.json();
                const { ph, temperature, turbidity } = data;

                // Atualiza os valores no monitor
                updateMonitor(ph, temperature, turbidity); 
            } catch (error) {
                console.error('Erro ao obter dados:', error);
            }
        }

        // Intervalo para atualizar os dados a cada 5 segundos
        setInterval(fetchData, 5000);

        // Chamada inicial para preencher os dados
        fetchData();
    </script>
</body>
</html>
)=====";