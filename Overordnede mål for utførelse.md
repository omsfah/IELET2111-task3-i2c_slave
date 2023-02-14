# Oppgavekrav 
- MÅ: Lage en I2C slave, enten egenkodet eller via rammeverk.
- MÅ: Noden skal overvåke både egen og ekstern spenning
- MÅ: Noden skal overvåke temperatur via en termistor.
- MÅ: Noden skal bed hjelp av lys- og fotodiode bestemme hastigheten til ekstern vifte.
- MÅ: Via feilutganger vil noden varsle både høy temperatur, og gi feil dersom begge viftene står stille i for lang tid, eller temperaturen er for høy.
- MÅ: Endringer i innstillinger må kunne gjøres over I2C og lagres på kontrolleren.
- BØR: Viften skal overvåkes over tid, slik at avvik i viftehastighet og evt varsler kan leses av via I2C.
- Tilleggskretser kan brukes etter behov. EEPROM / ADC / OPAMP osv.

# Gruppens ønsker
- Tilvirke i2c-slave, helst på egen PCB med 4+ adressebits, som er mulig å endrefor sluttbruker.
  - underordnet burde i2c slaven ha en analog avlesning (0-5v) og en analog ut (0-5v) og potensielt en pwm digital ut.
  - Hurtigconnector for å daisychaine i2c slaver. (Dette for fleksibilitet)
- For speningsoveråkning kan man benytte 2 i2c nde, en for å måle bus spenning, en for å måle eksternspenning
- For temperatur kan vi benytte en termistor, evt type k element eller ic med temp avlesning (Pick your poison)
- For lys/ fotodiode kan man vurdere polariseringfor å minimere feildeteksjon. evt lasersensor
- Feilutganger er jeg usikker på hvordan vi skal løse
- I2C endringer er jeg usikker på
- Tidsovervåking krever et lagringssted for dataene

# Utfordringer

# Forventet resultat

# Prosjektrapport
Skrives i latex, antar vi legger latex fila på git.
