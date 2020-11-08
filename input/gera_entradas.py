import sys
import random
from faker import Faker


def gera(nLinhas=100, nCampos=None):
    with open(f"{path}/file{nLinhas}-{nCampos}_python.txt", "w+", encoding="utf8") as file:
        if not nCampos:
            nCampos = random.randint(2, 10)

        camposFuncs = [
            fake.name,
            fake.date,
            fake.ssn,
            fake.ascii_email,
            fake.job,
            fake.phone_number,
            fake.coordinate,
            fake.license_plate,
            fake.credit_card_expire,
        ][:nCampos]

        for _ in range(nLinhas):
            file.write(f"{random.randint(0, 999999)},")
            for funcao in camposFuncs[:-1]:
                file.write(f"{funcao()},")
            file.write(camposFuncs[-1]())
            file.write("\n")


if __name__ == "__main__":
    fake = Faker("pt_BR")
    path = "python/"

    try:
        nLinhas = int(sys.argv[1])
        nCampos = int(sys.argv[2])
    except:
        nLinhas = 1000
        nCampos = 10

    gera(nLinhas, nCampos)
