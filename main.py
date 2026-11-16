DIGITOS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+/"

def bases(opcao):
    # Traduz a opção escolhida no menu para o valor numérico da base.
    opcoes = {
        1: 2,
        2: 16,
        3: 8,
        4: 10,
        5: 64,
        6: 20,
    }
    return opcoes[int(opcao)]

def desconverter(num, base):
    # Interpreta o número escrito em uma base e retorna seu valor decimal.
    # O sinal é tratado separadamente; cada dígito é acumulado na base recebida.
    texto = str(num).strip()
    sinal = -1 if texto.startswith("-") else 1
    
    total = 0
    
    for caractere in texto.lstrip("+-"):
        # Em bases até 36 letras são aceitas sem diferenciar maiúsculas/minúsculas
        if base <= 36:
            caractere = caractere.upper()      
        valor = DIGITOS.find(caractere)
        total = total * base + valor

    return sinal * total

def converter(num, base):
    # Converte um inteiro decimal para sua representação na base indicada.
    numero = int(num)
    if numero == 0:
        return "0"

    sinal = "-" if numero < 0 else ""
    numero = abs(numero)
    resultado = []
    while numero:
        resultado.append(DIGITOS[numero % base])
        numero //= base 

    return sinal + "".join(reversed(resultado))

# Menu principal em loop para q possa fazer varias operações de conversão
while(True):
    menu = "[1] Binário\n[2] Hexadecimal\n[3] Octal\n[4] Decimal\n[5] Base 64\n[6] Vigesimal\n[0] Sair\n"
    
    entrada1 = int(input(f"Escolha a base de entrada: \n{menu}"))
    
    # Tratamento da entrada 1
    if entrada1 >= 1 and entrada1 <= 6:
        base_entrada = bases(entrada1) #chamamos a funcao bases para termos a base oficial da entrada
    elif entrada1 == 0:
        print('Saindo')
        break
    else: 
        print("\033[31mNumero invalido vamos reiniciar\033[37m")
        continue

    
    numero = input("Digite o seu número: ")
    #Sempre que num numero chega ele é convertido para decimal
    #Passamos para varivael valor_decimal o valor q recebemos ao chamar a funcao desconverter passando a entrada e a base oficial
    valor_decimal = desconverter(numero, base_entrada) 

    # Tratamento da entrada 2
    entrada2 = input(f"Escolha a base de saída:\n{menu}")
    if entrada1 >= 1 and entrada1 <= 6:
            base_saida = bases(entrada2) #chamamos a funcao bases para termos a base oficial da saida
    elif entrada2 == 0:
        print('saindo')
        break
    else: 
        print("\033[31mNumero invalido vamos reiniciar\033[37m")
        continue
    
    # Mostramos o valor final chamando a funcao converter que vai converter o numero de entrada convertido em decimal para o numero da base escolhida
    print(f"\033[32m Resposta: {converter(valor_decimal, base_saida)}\033[37m") 
