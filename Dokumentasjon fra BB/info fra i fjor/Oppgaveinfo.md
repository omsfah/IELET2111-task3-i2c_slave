# Informasjon 
Viftene som skal monitoreres er inne i USRP-modulene. Viftene er av type Emb-papst 252H. Datablad på de finnes under.
Spenningen og strømmen som skal monitoreres er inn på USRP-en. Data for strøm og spenning finnes i datablad for USRP-en.
Elprolab har laget en egen utestet løsning monitorering av viftehastighet. Data om denne ligger i egen mappe. 
Strømforsyningen som gir strøm til mikrokontrollerkretsen er en HNP72 som gir ut 12V. Slik dette er koblet så deler mikrokontrolleren på strømlinjene til en 12V kontaktor (stort relè). 
Kontaktoren igjen kobler inn og ut 230V siden av strømforsyningen til USRP-en. Strømforsyningen til USRP-en er en SDI90-12-U eller ETSA120750UD avhengig om den er byttet eller ikke. 
Noen viktige momenter:
Siden strømforsyningen til USRP-en har kondensatorer i seg, vil mikrokontrolleren måle spenning inn fra strømforsyningen til USRP-en en liten tid etter mikrokontrolleren selv har mistet spenning. Dette må tas hensyn til. (spør om dere er i tvil hva dette innebærer)
Siden mikrokontrolleren deler 12V med en kontaktor vil det kunne komme transienter fra spolen i kontaktoren når strømmen slås av.
Det er kun 12V som er tilgjengelig, så dette må reguleres ned både for strømforsyningen til mikrokontrolleren og målesignalene inn. 
# Spørsmål og svar
Hva mener med "Node" i oppgaveteksten?
Noden er oppgaven deres. Altså mikrokontrolleren og kretsen rundt. Grunnen til at det kalles node er at mikrokontrolleren er del av et større system av I2C. Men tenk på noden som oppgaven deres. (Dere skal bare lage en kopi av den)
Skal vi styre viftehastigheten?
Nei. I denne oppgaven skal dere kun overvåke og evt gi ut et digitalt alarmsignal om f.eks. begge viftene har stoppet.
Hva er spenningen vi skal overvåke?
Dere skal overvåke to spenninger. Den ene er spenningen som brukes av mikrokontrolleren, den andre brukes av utstyret viftene står i, altså USRP-en.
Disse spenningene kommer fra forskjellige strømforsyninger.
Begge strømforsyningene er i utgangspunktet ca 12V. Men feil-modier kan medføre høyere spenninger. Dere kan sette grense på maks 24V feilspenning.

Hvilken hastighet har viftene? Det er oppgitt i datablad hva viftene skal ha, dersom de ikke har feil på seg. Men det vil forekomme variasjon etter hvilke luftstrømmer som er og produksjonsavvik. Men større avvik far dette, spesielt lavere turtall, er ofte indikasjon på feil. Her kan det være lurt å la det være konfigurerbart hvilket nivå som er feil. (gjerne vi I2C om dere velger det)
Hvilken adresse har mikrokontrolleren?
Det velger dere. Men kan være lurt å la det være konfigurerbart ved hjelp av variabler. Mikrokontrolleren har kun en adresse. Hvordan evt konfigurasjon settes og hvordan data leses av vider er opp til dere. Men det vil trolig bli en liten protokoll på toppen av I2C. I så fall trenger den ikke være avansert.
*Merk.
Enheten dere skal måle viftehastigheten til kan slås av. Det vil si at dere kan måle spenning inn på USRP-en, men 0 i viftehastighet, uten at det er en feil. Det er rett og slett enheten som står av. Hvordan dere skal skille dette tilfeller fra tilfellet hvor viftene har stoppet, men enheten står på, er å måle strømmen inn på enheten. Dersom det går strøm inn på enheten, OG viftene står stille, står enheten på og viftene har feilet. ( gitt at de har fått nok tid til å spinne opp)
