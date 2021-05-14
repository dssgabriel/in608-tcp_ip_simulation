#include "Ordinateur.hpp"

uint8_t Ordinateur::m_NbrOrdinateur = 0;

// Constructeurs
Ordinateur::Ordinateur() : Machine() {
    m_NbrOrdinateur++;
    m_IdOrdinateur = m_NbrOrdinateur;

    m_Nom = "Ordinateur" + std::to_string(m_IdOrdinateur);

    m_ControleCongestion.clear();
    m_TempsTraitementPaquet.clear();
}

// // Ordinateur(const std::string& nom) {}

// Destructeur
Ordinateur::~Ordinateur() {

}

// Getters
const uint8_t& Ordinateur::getNbrOrdinateur() const {
    return m_NbrOrdinateur;
}

const uint8_t& Ordinateur::getIdOrdinateur() const {
    return m_IdOrdinateur;
}

const std::vector<ElementControleCongestion>& Ordinateur::getControleCongestion() const {
    return m_ControleCongestion;
}
    
const ElementControleCongestion& Ordinateur::getControleCongestion(const int& position) const {
    if (position < 0 || position > (int)m_ControleCongestion.size()) {
        std::cout << "ERREUR : position en dehors du tableau.\n";
        exit(EXIT_FAILURE);
    }

    return m_ControleCongestion[position];
}

const std::map<uint16_t, double> Ordinateur::getTempsTraitementPaquet() const {
    return m_TempsTraitementPaquet;
}

double Ordinateur::getTempsTraitementPaquet(const uint16_t& cle) const {
    auto trouve = m_TempsTraitementPaquet.find(cle);
    if (trouve != m_TempsTraitementPaquet.end()) {
        return trouve->second;
    } else {
        std::cout << "ERREUR : Aucun paquet ne correspond au numero demandé.\n";
        return -1;
    }
}

// ################ //
// Methodes externe //
// ################ //
std::bitset<16> genererMessage() {
    uint16_t tmp = rand() % 2^16;
    return std::bitset<16> (tmp);
}

std::deque<std::stack<std::bitset<16>>> convertirQueueDeque(
    std::queue<std::stack<std::bitset<16>>*> queue)
{
    std::cout << "Debut convertirQueueDeque\n";
    std::deque<std::stack<std::bitset<16>>> dequeu;
    std::cout << "dequeu creer\n";
    afficher(queue);

    for (size_t i = 0; i < queue.size(); i++) {
        std::cout << "push_back\n";
        std::cout << "queue.front() = " << queue.front();
        afficher(queue);
        dequeu.push_back(*queue.front());
        std::cout << "pop\n";
        queue.pop();
    }

    std::cout << "Fin convertirQueueDeque\n";
    return dequeu;   
}

bool tripleACK(std::queue<std::stack<std::bitset<16>>*> donnees,
                std::bitset<32>& ackTriple)
{
    // Initialisation des couches.
    Physique couchePhy;
    Internet coucheInt;
    Transport coucheTrans;

    // Convertion de la file de donnees en double file.
    std::deque<std::stack<std::bitset<16>>> donneesDeque;
    donneesDeque = convertirQueueDeque(donnees);

    // On regarde un ack.
    for (int i = 0; i < int(donneesDeque.size()) - 2; ++i) {
        // Initialisation des couches.
        Physique couchePhy2;
        Internet coucheInt2;
        Transport coucheTrans2;

        // On desencapsule.
        std::stack<std::bitset<16>> paquet = couchePhy.desencapsuler(donneesDeque[i]);
        std::stack<std::bitset<16>> segment = coucheInt.desencapsuler(paquet);
        std::bitset<16> donnee = coucheTrans.desencapsuler(segment);

        // On regarde un deuxieme ack.
        for (int j = i + 1; j < int(donneesDeque.size()) - 1; ++j) {
            // Initialisation des couches.
            Physique couchePhy3;
            Internet coucheInt3;
            Transport coucheTrans3;

            // On desencapsule.
            std::stack<std::bitset<16>> paquet2 = couchePhy2.desencapsuler(donneesDeque[j]);
            std::stack<std::bitset<16>> segment2 = coucheInt2.desencapsuler(paquet2);
            std::bitset<16> donnee2 = coucheTrans2.desencapsuler(segment2);
            
            // 2 ack.
            if (coucheTrans.getAck2() == coucheTrans2.getAck2()) {
                for (int k = j + 1; k < int(donneesDeque.size()); ++k) {
                    // On desencapsule.
                    std::stack<std::bitset<16>> paquet3 = couchePhy3.desencapsuler(donneesDeque[k]);
                    std::stack<std::bitset<16>> segment3 = coucheInt3.desencapsuler(paquet3);
                    std::bitset<16> donnee3 = coucheTrans3.desencapsuler(segment3);

                    // Si le premier ack est equivalent au troisieme,
                    // nous avons un triple ack.
                    if (coucheTrans.getAck2() == coucheTrans3.getAck2()) {
                        ackTriple = coucheTrans.getAck2();

                        // On re encapsule.
                        segment3 = coucheTrans3.encapsuler(donnee3);
                        paquet3 = coucheInt3.encapsuler(segment3);
                        donneesDeque[k] = couchePhy3.encapsuler(paquet3);

                        return true;
                    } else {
                        // On re encapsule.
                        segment3 = coucheTrans3.encapsuler(donnee3);
                        paquet3 = coucheInt3.encapsuler(segment3);
                        donneesDeque[k] = couchePhy3.encapsuler(paquet3);
                    }
                }
            }

            // On re encapsule.
            segment2 = coucheTrans2.encapsuler(donnee2);
            paquet2 = coucheInt2.encapsuler(segment2);
            donneesDeque[j] = couchePhy2.encapsuler(paquet2);
        }

        // On re encapsule.
        segment = coucheTrans.encapsuler(donnee);
        paquet = coucheInt.encapsuler(segment);
        donneesDeque[i] = couchePhy.encapsuler(paquet);
    }

    return false;
}

int calculerNombreAck(std::deque<std::stack<std::bitset<16>>> donneesDeque) {
    int compteur = 0;
    Physique couchePhy;
    Internet coucheInt;
    Transport coucheTrans;

    // On parcours les ack.
    for (int i = 0; i < int(donneesDeque.size()); ++i) {

        // On desencapsule.
        std::stack<std::bitset<16>> paquet = couchePhy.desencapsuler(donneesDeque[i]);
        std::stack<std::bitset<16>> segment = coucheInt.desencapsuler(paquet);
        std::bitset<16> donnee = coucheTrans.desencapsuler(segment);

        // On recupere l'ack, si il est different de 0, on incremente le nombre d'ack.
        if (coucheTrans.getAck2().to_ulong() != 0) {
            compteur++;
        }

        // On re encapsule.
        segment = coucheTrans.encapsuler(donnee);
        paquet = coucheInt.encapsuler(segment);
        donneesDeque[i] = couchePhy.encapsuler(paquet);
    }

    return compteur;
}

bool timeout(std::stack<std::bitset<16>> paquet) {
    // On desencapsule.
    Internet coucheInt;
    std::stack<std::bitset<16>> segment = coucheInt.desencapsuler(paquet);

    // Si le TTL est a 0, alors le paquet est en timeout.
    if (coucheInt.getTTL().to_ulong() == 0) {
        // On re encapsule.
        paquet = coucheInt.encapsuler(segment);

        return true;
    } else {
        // On re encapsule.
        paquet = coucheInt.encapsuler(segment);
    }

    return false;
}

bool estDuplique(std::deque<std::stack<std::bitset<16>>> donneesDeque,
        const std::bitset<32>& ack)
{
    // Initialisation des couches.
    Physique couchePhy;
    Internet coucheInt;
    Transport coucheTrans;

    // On regarde un ack.
    for (int i = 0; i < int(donneesDeque.size()) - 2; ++i) {
        // On desencapsule.
        std::stack<std::bitset<16>> paquet = couchePhy.desencapsuler(donneesDeque[i]);
        std::stack<std::bitset<16>> segment = coucheInt.desencapsuler(paquet);
        std::bitset<16> donnee = coucheTrans.desencapsuler(segment);

        if (coucheTrans.getAck2() == ack) {

            // On re encapusle.
            segment = coucheTrans.encapsuler(donnee);
            paquet = coucheInt.encapsuler(segment);
            donneesDeque[i] = couchePhy.encapsuler(paquet);            

            return true;
        } else {

            // On re encapusle.
            segment = coucheTrans.encapsuler(donnee);
            paquet = coucheInt.encapsuler(segment);
            donneesDeque[i] = couchePhy.encapsuler(paquet);            
        }
    }

    return false;
}

std::stack<std::bitset<16>> trouverDonnee(
        std::queue<std::stack<std::bitset<16>>*> donnees,
        const std::bitset<32>& seq)
{
    //
    Physique couchePhy;
    Internet coucheInt;
    Transport coucheTrans;

    // Convertion de la file de donnees en double file.
    std::deque<std::stack<std::bitset<16>>> donneesDeque;
    donneesDeque = convertirQueueDeque(donnees);    

    //
    for (int i = 0; i < int(donneesDeque.size()); ++i) {
        // On desencapsule.
        std::stack<std::bitset<16>> paquet = couchePhy.desencapsuler(donneesDeque[i]);
        std::stack<std::bitset<16>> segment = coucheInt.desencapsuler(paquet);
        std::bitset<16> donnee = coucheTrans.desencapsuler(segment);

        // on verifie que ce soit la bonne trame.
        if (coucheTrans.getSeq() == seq) {

            // Je re encapsule.
            segment = coucheTrans.encapsuler(donnee);
            paquet = coucheInt.encapsuler(segment);
            donneesDeque[i] = couchePhy.encapsuler(paquet);

            return donneesDeque[i];
        }
    }

    std::cout << "ERREUR : Dans la fonction 'trouverDonnee' : Aucune donnée\n";
    exit(EXIT_FAILURE); 
}

// ######## //
// Methodes //
// ######## //
void Ordinateur::remplirFileDonnees(
    const ParamInterface& config, const MAC& destination) 
{
    // Preparation des donnees
    for (size_t i = 0; i < 1; ++i) {
        // Initialisation
        Transport coucheTrans;
        Internet coucheInt;
        Physique couchePhy;

        // Encapsulation couche Transport
        coucheTrans.setPortSrc(coucheTrans.portAlea());
        coucheTrans.setPortDest(config.m_TypeFichier);
        coucheTrans.setCwnd(std::bitset<16>(0));
        coucheTrans.setSyn(std::bitset<16>(1));
        coucheTrans.setAck1(std::bitset<16>(0));
        coucheTrans.setSeq(std::bitset<32>(i+1));
        coucheTrans.setAck2(std::bitset<32>(0));
        coucheTrans.calculerChecksum(); // <=> setChecksum
        std::stack<std::bitset<16>> segment;
        segment = coucheTrans.encapsuler(genererMessage());
        std::cout << coucheTrans;

        // Encapsulation couche Internet
        coucheInt.setIpSrc(config.m_Source);
        coucheInt.setIpDest(config.m_Destination);
        coucheInt.setTTL(std::bitset<8> (100));
        coucheInt.setProtocoleId();
        coucheInt.calculerChecksum();
        std::stack<std::bitset<16>> paquet;
        paquet = coucheInt.encapsuler(segment);
        std::cout << coucheInt;

        // Encapsulation couche Physique
        couchePhy.setMacSrc(m_Mac);
        couchePhy.setMacDest(destination);
        std::stack<std::bitset<16>> trame;
        trame = couchePhy.encapsuler(paquet);
        std::cout << couchePhy;
        
        //
        setDonnee(&trame);
    }

    afficher(m_FileDonnees);
}

void Ordinateur::synchroniser() {
    // TODO : A faire
}

void Ordinateur::finDeSession() {
    // TODO : A faire
}

void Ordinateur::envoyer(const uint32_t cwnd) {
    std::cout << "Debut envoie\n";
    // Trouver la machine voisine.
    // Une seule machine voisine pour un ordinateur (routeur ou commutateur).
    Machine* voisine = m_Voisins.front();

    //
    for (int i = 0; i < int(cwnd); ++i) {
        // Vide la file de donnees.
        std::stack<std::bitset<16>>* donneeRecu = m_FileDonnees.front();
        m_FileDonnees.pop();
        
        // Initialisation des couches.
        Physique couchePhy;
        Internet coucheInt;
        Transport coucheTrans;

        // On desencapsule.
        std::stack<std::bitset<16>> paquet = couchePhy.desencapsuler(*donneeRecu);
        std::stack<std::bitset<16>> segment = coucheInt.desencapsuler(paquet);
        std::bitset<16> donnee = coucheTrans.desencapsuler(segment);

        // Traitement de la donnee.
        traitement(*donneeRecu, voisine->getMac());

        // On re encapsule.
        segment = coucheTrans.encapsuler(donnee);
        paquet = coucheInt.encapsuler(segment);
        *donneeRecu = couchePhy.encapsuler(paquet);

        // La machine suivante recois le paquet
        voisine->setDonnee(donneeRecu);
    }

    //
    std::cout << "Recevoir\n";
    voisine->recevoir();
}

void Ordinateur::recevoir() {
    std::cout << "Debut recevoir\n";
    //
    std::deque<std::stack<std::bitset<16>>> donneesDeque;
    donneesDeque = convertirQueueDeque(m_FileDonnees);

    //
    for (int i = 1; i < int(donneesDeque.size()); ++i) {
        //
        Physique couchePhy, couchePhy2;
        Internet coucheInt, coucheInt2;
        Transport coucheTrans, coucheTrans2;

        // On desencapsule.
        std::stack<std::bitset<16>> paquet2 = couchePhy2.desencapsuler(donneesDeque[i]);
        std::stack<std::bitset<16>> segment2 = coucheInt2.desencapsuler(paquet2);
        std::bitset<16> donnee2 = coucheTrans2.desencapsuler(segment2);

        // On desencapsule.
        std::stack<std::bitset<16>> paquet = couchePhy.desencapsuler(donneesDeque[i-1]);
        std::stack<std::bitset<16>> segment = coucheInt.desencapsuler(paquet);
        std::bitset<16> donnee = coucheTrans.desencapsuler(segment);

        //
        if (coucheTrans.getSeq().to_ulong() + 1 != coucheTrans2.getSeq().to_ulong()) {
            std::stack<std::bitset<16>> perdu;
            std::bitset<32> paquetPerdu = coucheTrans2.getSeq();
            std::bitset<16> paquetPerduGauche, paquetPerduDroit;
            diviser(paquetPerdu, paquetPerduGauche, paquetPerduDroit);
            perdu.push(paquetPerduGauche);
            perdu.push(paquetPerduDroit);
            setDonnee(&perdu);
        }

        // On re encapusle.
        segment = coucheTrans.encapsuler(donnee);
        paquet = coucheInt.encapsuler(segment);
        donneesDeque[i-1] = couchePhy.encapsuler(paquet);

        segment2 = coucheTrans2.encapsuler(donnee2);
        paquet2 = coucheInt2.encapsuler(segment2);
        donneesDeque[i] = couchePhy2.encapsuler(paquet2);
    }

    // donneesDeque.clear();
}

void Ordinateur::traitement(std::stack<std::bitset<16>> &trame, MAC nouvelleDest) {
    std::cout << "Debut traitement\n";
    // Recuperation du paquet.
    Physique couchePhy;
    std::stack<std::bitset<16>> paquet = couchePhy.desencapsuler(trame);
    
    // Recuperation adresse MAC destination.
    MAC ancienneDest = couchePhy.getMacDest();

    // Changement adresse MAC.
    // La destination devient la source.
    // Ajout nouvelle destination.
    couchePhy.setMacSrc(ancienneDest);
    couchePhy.setMacDest(nouvelleDest);

    // Encapsulation.
    trame = couchePhy.encapsuler(paquet);
}

void Ordinateur::slowStart(std::bitset<16>& cwnd, uint16_t& ssthresh1) {
    std::cout << "Debut slowStart\n";
    // Convertion de la file de donnees en double file.
    std::deque<std::stack<std::bitset<16>>> donneesDeque;
    donneesDeque = convertirQueueDeque(m_FileDonnees);

    //
    uint64_t cwndConvert = cwnd.to_ulong();
    if(cwndConvert < ssthresh1) {
        std::cout << cwndConvert << " < " << ssthresh1 << std::endl;
        for(int i = 0; i < int(donneesDeque.size()); ++i) {
            //
            Physique couchePhy;
            Internet coucheInt;
            Transport coucheTrans;

            // On desencapsule.
            std::stack<std::bitset<16>> paquet = couchePhy.desencapsuler(donneesDeque[i]);
            std::stack<std::bitset<16>> segment = coucheInt.desencapsuler(paquet);
            std::bitset<16> donnee = coucheTrans.desencapsuler(segment);
            
            if(!estDuplique(donneesDeque ,coucheTrans.getAck2())) {
                if(cwndConvert >= ssthresh1) {
                    std::cout << cwndConvert << " >= " << ssthresh1 << std::endl;
                    std::cout << "On entre dans congestionAvoidance(" << cwnd << ")\n";
                    
                    //
                    coucheTrans.setCwnd(cwnd);

                    // On re encapusle.
                    segment = coucheTrans.encapsuler(donnee);
                    paquet = coucheInt.encapsuler(segment);
                    donneesDeque[i] = couchePhy.encapsuler(paquet);

                    //
                    congestionAvoidance(cwnd);
                    return;
                }
                else {
                    cwndConvert *= 2;
                    std::cout << "cwndConvert *= 2, cwndConvert = " << cwndConvert << std::endl;
                    cwnd = std::bitset<16> (cwndConvert);
                }
            }
            //
            coucheTrans.setCwnd(cwnd);

            // On re encapusle.
            segment = coucheTrans.encapsuler(donnee);
            paquet = coucheInt.encapsuler(segment);
            donneesDeque[i] = couchePhy.encapsuler(paquet);
        }
        //
        std::cout << "On envoie\n";
        envoyer(cwndConvert);
    }
    else {
        std::cout << "On entre dans congestionAvoidance2(" << cwnd << ")\n";
        congestionAvoidance(cwnd);
        return;
    }
    std::cout << "Fin slowStart\n";
}

void Ordinateur::congestionAvoidance(std::bitset<16>& cwnd) {
    // Convertion de la file de donnees en double file.
    std::deque<std::stack<std::bitset<16>>> donneesDeque;
    donneesDeque = convertirQueueDeque(m_FileDonnees);
    int nombreAck = calculerNombreAck(donneesDeque);

    Physique couchePhy;
    uint64_t cwndConvert = cwnd.to_ulong();
    std::bitset<32> ackTriple;

    //
    if (tripleACK(m_FileDonnees, ackTriple)) {
        int ackTripleConvert = int(ackTriple.to_ulong());
        fastRetransmit(std::bitset<32> (ackTripleConvert - 1), cwnd);
        return;
    }
    
    //
    for (int i = 0; i < nombreAck; ++i) {
        cwndConvert += 1;
        cwnd = std::bitset<16> (cwndConvert);

        //
        std::stack<std::bitset<16>> paquet = couchePhy.desencapsuler(donneesDeque[i]);
        if (timeout(paquet)) {
            uint16_t ssthresh = cwndConvert / 2;
            cwndConvert = 1;
            cwnd = std::bitset<16> (cwndConvert);
            donneesDeque[i] = couchePhy.encapsuler(paquet);
            slowStart(cwnd, ssthresh);
            return;
        } else {
            donneesDeque[i] = couchePhy.encapsuler(paquet);
        }
    }
}

void Ordinateur::fastRetransmit(const std::bitset<32>& seq, std::bitset<16>& cwnd) {
    std::stack<std::bitset<16>> s = trouverDonnee(m_FileDonnees, seq);
    setDonnee(&s);
    fastRecovery(cwnd);
}

void Ordinateur::fastRecovery(std::bitset<16>& cwnd) {
    //
    uint64_t cwndConvert = cwnd.to_ulong();
    uint16_t ssthresh2 = cwndConvert / 2;
    cwndConvert = ssthresh2 + 3;
    cwnd = std::bitset<16>(cwndConvert);

    // Convertion de la file de donnees en double file.
    std::deque<std::stack<std::bitset<16>>> donneesDeque;
    donneesDeque = convertirQueueDeque(m_FileDonnees);

    //
    for (int i = 0; i < int(donneesDeque.size()); ++i) {
        // Initialisation des couches.
        Physique couchePhy;
        Internet coucheInt;
        Transport coucheTrans;

        // On desencapsule.
        std::stack<std::bitset<16>> paquet = couchePhy.desencapsuler(donneesDeque[i]);
        std::stack<std::bitset<16>> segment = coucheInt.desencapsuler(paquet);
        std::bitset<16> donnee = coucheTrans.desencapsuler(segment);

        //
        if (coucheTrans.getAck2() == 0) {
            continue;
        }
        if (estDuplique(donneesDeque, coucheTrans.getAck2())) {
            cwndConvert += 1;
            cwnd = std::bitset<16>(cwndConvert);
        } else {
            cwndConvert = ssthresh2;
            cwnd = std::bitset<16>(cwndConvert);
            
            // On re encapusle.
            segment = coucheTrans.encapsuler(donnee);
            paquet = coucheInt.encapsuler(segment);
            donneesDeque[i] = couchePhy.encapsuler(paquet); 

            congestionAvoidance(cwnd);
            return;
        }
    }
}