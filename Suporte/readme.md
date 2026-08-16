## 🧪 Ferramentas auxiliares

Esta seção possui ferramentas auxiliares usadas no processo de desenvolvimento e testes da ferramenta.

Scanner_DHT: Efetua Leitura de um sensor DHT11, procurando por NaN's para verificar se ele inicializou corretamnete 

Scanner_i2c: procura por sensores que usam o protocolo i2c (display OLED, BMP e TSL no caso) e retorna seus endereços, útil para identificar falhas de pinagem na montagem destes sensores

Scanner_OLED: Procura especificamente por um display oled e testa se ele responde e retorna dados na tela

