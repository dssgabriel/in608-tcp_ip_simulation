#include "Contexte.hpp"

// Constructeur
Contexte::Contexte() {
    // Chargement du reseau Simple par defaut
    m_Reseau = nullptr;
    m_Config.m_Source = {192, 240, 20, 1};
    m_Config.m_Destination = {168, 4, 125, 6};
    m_Config.m_NbPaquet = 500;
    m_Config.m_Ssthresh = 153;
    m_Config.m_TypeFichier = FTP;
    chargerConfig(1);
    m_Temps = 0;
    m_TabCongestion = new std::vector<ElementControleCongestion>();
    // Test
    std::cout << m_Reseau->getNom() << "\n";
}

ReseauGraphe* Contexte::getReseau() {
    return m_Reseau.get();
}

ParamInterface& Contexte::getConfig() {
    return m_Config;
}

MAC& Contexte::getMACArrivee() {
    return m_Destination;
}

int64_t& Contexte::getTemps() {
    return m_Temps;
}

void Contexte::setReseau(std::unique_ptr<ReseauGraphe>& newRes) {
    m_Reseau = std::move(newRes);
}

void Contexte::charger() {
    //mise a jour de l'affichage
    if(m_Reseau->getNom() == "ReseauSimple") {
        AffichageReseau::GetInstance().configSimple();
    }
    else if(m_Reseau->getNom() == "ReseauMaison") {
        AffichageReseau::GetInstance().configMaison();
    }
    else if(m_Reseau->getNom() == "ReseauPme") {
        AffichageReseau::GetInstance().configPme();
    }
    else if(m_Reseau->getNom() == "ReseauEntreprise") {
        AffichageReseau::GetInstance().configEntreprise();
    }
    //ChoixReseau::GetInstance().analyseConfig();
}

void Contexte::sauvegarder() {

}

void Contexte::exporterGraphe(const QString& nomFichier) {
    AffichageReseau::GetInstance().sauvegarderGraphe(nomFichier);
}

std::string Contexte::informationsReseau() {
    return "test";
}

void Contexte::chargerConfig(int numConfig) {
    if(numConfig == 1) {
        m_Reseau = chargerReseau("../src/include/configReseau/ReseauSimple.json");
        AffichageReseau::GetInstance().configSimple();
    }
    else if(numConfig == 2) {
        m_Reseau = chargerReseau("../src/include/configReseau/ReseauMaison.json");
        AffichageReseau::GetInstance().configMaison();
    }
    else if(numConfig == 3) {
        m_Reseau = chargerReseau("../src/include/configReseau/ReseauPme.json");
        AffichageReseau::GetInstance().configPme();
    }
    else if(numConfig == 4) {
        m_Reseau = chargerReseau("../src/include/configReseau/ReseauEntreprise.json");
        AffichageReseau::GetInstance().configEntreprise();
    }
}

void Contexte::executerSimulation() {
    ConfigReseau::GetInstance().initialiserTexte();
    AffichageReseau::GetInstance().initialiserGraphe();
}

void Contexte::rafraichir() {

}

void Contexte::stopSimulation() {

}

// Destructeur //
Contexte::~Contexte() {
    if(m_TabCongestion) { delete m_TabCongestion;}
}
