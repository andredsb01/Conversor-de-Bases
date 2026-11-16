#include <iostream>
#include <limits>
#include <string>

// converte um caractere em seu valor numérico '0'..'9' -> 0..9,
// 'A'..'Z' (ou 'a'..'z') 10..35. Retorna -1 se não for um dígito
int valor_do_digito(char caractere) {
    if (caractere >= '0' && caractere <= '9') {
        return caractere - '0';
    }
    if (caractere >= 'A' && caractere <= 'Z') {
        return caractere - 'A' + 10;
    }
    if (caractere >= 'a' && caractere <= 'z') {
        return caractere - 'a' + 10;
    }
    return -1;
}

// lê um número escrito na base informada e devolve seu valor decimal
// o cálculo verifica os dígitos e detecta estouro de unsigned long long
bool para_decimal(const std::string& numero, int base,
                  unsigned long long& resultado) {
    if (numero.empty() || base < 2 || base > 36) {
        return false;
    }

    resultado = 0;
    for (std::size_t i = 0; i < numero.size();) {
        int digito;
        if (numero[i] == '(') {
            // nas bases acima de 36, aceita dígitos explícitos como (42)
            const std::size_t fechamento = numero.find(')', i + 1);
            if (fechamento == std::string::npos || fechamento == i + 1) {
                return false;
            }
            try {
                const std::string texto_digito =
                    numero.substr(i + 1, fechamento - i - 1);
                std::size_t caracteres_lidos = 0;
                const unsigned long long valor =
                    std::stoull(texto_digito, &caracteres_lidos);
                if (caracteres_lidos != texto_digito.size() ||
                    valor > static_cast<unsigned long long>(base - 1)) {
                    return false;
                }
                digito = static_cast<int>(valor);
                i = fechamento + 1;
            } catch (...) {
                return false;
            }
        } else {
            digito = valor_do_digito(numero[i]);
            ++i;
        }
        if (digito < 0 || digito >= base) {
            return false; // o dígito não existe na base escolhida
        }

        // confere antes de multiplicar e somar para evitar estouro.
        const auto limite = std::numeric_limits<unsigned long long>::max();
        if (resultado > (limite - static_cast<unsigned long long>(digito)) /
                            static_cast<unsigned long long>(base)) {
            return false;
        }
        resultado = resultado * static_cast<unsigned long long>(base) +
                    static_cast<unsigned long long>(digito);
    }
    return true;
}

// escreve um valor decimal na base desejada, usando A-Z para valores 10-35
std::string de_decimal(unsigned long long numero, int base) {
    const std::string digitos = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (numero == 0) {
        return "0";
    }

    std::string resultado;
    while (numero > 0) {
        const int resto = static_cast<int>(numero % base);
        resultado.insert(resultado.begin(), digitos[resto]);
        numero /= base;
    }
    return resultado;
}

// converte a opção do menu (1 a 6) para a base correspondente
int verificar_base(int opcao) {
    switch (opcao) {
        case 1: return 2;  // Binária
        case 2: return 16; // Hexadecimal
        case 3: return 8;  // Octal
        case 4: return 10; // Decimal
        case 5: return 60; // Sexagesimal
        case 6: return 20; // Vigesimal
        default: return 0;
    }
}

// mstra as bases disponíveis no programa.
void mostrar_menu() {
    std::cout << "[1] Binario\n[2] Hexadecimal\n[3] Octal\n[4] Decimal\n"
                 "[5] Sexagesimal\n[6] Vigesimal\n[0] Sair\n";
}

int main() {
    while (true) {
        std::string entrada;
        int opcao_origem;
        int opcao_destino;

        std::cout << "\nDigite o numero que deseja converter: ";
        if (!(std::cin >> entrada)) {
            break; // Encerra caso a entrada seja fechada ou apresente erro
        }

        std::cout << "Escolha a base de origem:\n";
        mostrar_menu();
        if (!(std::cin >> opcao_origem)) {
            break;
        }
        if (opcao_origem == 0) {
            break;
        }

        const int base_origem = verificar_base(opcao_origem);
        if (base_origem == 0) {
            std::cout << "Opcao de base invalida.\n";
            continue;
        }

        std::cout << "Escolha a base de destino:\n";
        mostrar_menu();
        if (!(std::cin >> opcao_destino)) {
            break;
        }
        if (opcao_destino == 0) {
            break;
        }

        const int base_destino = verificar_base(opcao_destino);
        if (base_destino == 0) {
            std::cout << "Opcao de base invalida.\n";
            continue;
        }

        // Valida a representação na base de origem antes de converter
        unsigned long long valor_decimal;
        if (!para_decimal(entrada, base_origem, valor_decimal)) {
            std::cout << "Numero invalido para a base escolhida ou grande demais.\n";
            continue;
        }

        // Bases acima de 36 usam dígitos numéricos entre parênteses
        // por exemplo 125 na base 60 = (2)(5). A leitura aceita esse formato
        if (base_destino <= 36) {
            std::cout << "Resultado: " << de_decimal(valor_decimal, base_destino) << std::endl;
        } else if (valor_decimal == 0) {
            std::cout << entrada << "Resultado: 0" << std::endl;
        } else {
            std::string resultado;
            while (valor_decimal > 0) {
                const unsigned long long resto = valor_decimal % base_destino;
                resultado = "(" + std::to_string(resto) + ")" + resultado;
                valor_decimal /= base_destino;
            }
            std::cout << "Resultado: " << resultado << std::endl;
        }
    }

    std::cout << "Saindo\n";
    return 0;
}