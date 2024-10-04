import generatoreDiRichieste

def main():
    try:
        for x in range(0, 250):
            generatoreDiRichieste.genera()
            
    except KeyboardInterrupt:
        return
    
main()