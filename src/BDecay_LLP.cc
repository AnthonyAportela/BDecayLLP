// compile like main42

#include <set>
#include <string>
#include <array>
#include <iostream>
#include <cmath>
#include <functional>
#include "Config.hh"

#include "Pythia8/Pythia.h"

#include "Pythia8Plugins/HepMC2.h"

#include "HepMC/GenEvent.h"
#include "HepMC/IO_GenEvent.h"


namespace ALP {

    template<typename T>
    T fromString(const char*) {
        throw(std::runtime_error("Type not allowed"));
    }

    template<>
    int fromString(const char* value) {
        return atoi(value);
    }

    template<>
    size_t fromString(const char* value) {
        return atoi(value);
    }

    template<>
    double fromString(const char* value) {
        return atof(value);
    }

    template <>
    bool fromString(const char *value) {
        std::string tmp{value};
        if(tmp.find("true") != std::string::npos) {
            return true;
        }
        else if(tmp.find("false") != std::string::npos) {
            return false;
        }
        throw(std::runtime_error("Invalid bool"));
    }

    template<>
    std::string fromString(const char* value) {
        std::string tmp{value};
        return tmp;
    }

    template<typename T>
        bool parseArg(const char* arg, const std::string& name, T* value) {
        std::string tmp{arg};
        std::string tmpName = name + "=";
        if(tmp.find(tmpName) != std::string::npos) {
            auto pos = tmp.find("=");
            auto valcand = tmp.substr(pos+1);
            try{
                *value = fromString<T>(valcand.c_str());
            }
            catch(std::exception& e) {
                std::cout << "Parameter '" << name << "' not set: " << e.what() << std::endl;
                return false;
            }
            return true;
        }
        return false;
    }

} // namespace ALP

using namespace Pythia8;

using namespace std;

struct ModelParams {
    int id = 1500001;
    double mass = 0.3; //GeV
    double inclBRB = 1.e-7;
    double cTauCm = 10.; // meter
};

struct ExpParams {
    double absEtaMin = 0.;
    double absEtaMax = 4.;
    double minRCut = 1.;
    double maxRCut = 10.;
    double minELLP = -1.;
};

struct RunParams {
    //int nEventDecayed = 100000;
    //int nEventTried = 1000000;
    int nEventDecayed = 100;
    int nEventTried = 100000;
    double sqrtS = 14000.;
    int tune = 21;
    int seed = 7;
    string baseName = "MinBiasALP";
    string path = "./";
};

void initBDecays(Pythia8::Pythia& pythia, int llpCode, double llpmass) {
    double m2 = pow(llpmass, 2.);
    double lk = (1. - pow((llpmass - pythia.particleData.m0(130)) / pythia.particleData.m0(511), 2.)) * (1. - pow((llpmass + pythia.particleData.m0(130)) / pythia.particleData.m0(511), 2.));
    double fk = 1. / (1. - m2 / 38.);
    double lks = 0.;
    double fks = 0.;
    if(llpmass < pythia.particleData.m0(511) - pythia.particleData.m0(313)) {
        lks = (1. - pow((llpmass - pythia.particleData.m0(313)) / pythia.particleData.m0(511), 2.)) * (1. - pow((llpmass + pythia.particleData.m0(313)) / pythia.particleData.m0(511), 2.));
        fks = 3.65 / (1. - m2 / 28.) - 2.65 / (1. - m2 / 37.);
    }
    double btok = 0.03 * pow(fk, 2.) * sqrt(lk);
    double btoks = 0.04 * pow(fks, 2.) * pow(lks, 3. / 2.);
    double kfrac = btok/(btok + btoks);
    double ksfrac = btoks/(btok + btoks);
    if (llpmass < pythia.particleData.m0(511) - pythia.particleData.m0(130)) {
        // B0
        pythia.readString("511:onMode = 3");
        if(ksfrac > 1.e-6) {
            pythia.readString("511:addChannel = 2 " + to_string(kfrac) + "0 130 " + to_string(llpCode));
            pythia.readString("511:addChannel = 2 " + to_string(ksfrac) + "0 313 " + to_string(llpCode));
        }
        else {
            pythia.readString("511:addChannel = 2 1.00000 0 130 " + to_string(llpCode));
        }
    }
    if (llpmass < pythia.particleData.m0(521)  - pythia.particleData.m0(321)) {
        // B+
        pythia.readString("521:onMode = 3");
        if(ksfrac > 1.e-6) {
            pythia.readString("521:addChannel = 2 " + to_string(kfrac) + "0 321 " + to_string(llpCode));
            pythia.readString("521:addChannel = 2 " + to_string(ksfrac) + "0 323 " + to_string(llpCode));
        }
        else {
            pythia.readString("521:addChannel = 2 1.00000 0 321 " + to_string(llpCode));
        }
    }
    if (llpmass < pythia.particleData.m0(513) - pythia.particleData.m0(313)) {
        // Bstar0
        pythia.readString("513:onMode = 3");
        pythia.readString("513:addChannel = 2 1.00000 0 313 " + to_string(llpCode));
    }
    if (llpmass < pythia.particleData.m0(523) - pythia.particleData.m0(323)) {
        // Bstar+
        pythia.readString("523:onMode = 3");
        pythia.readString("523:addChannel = 2 1.00000 0 323 " + to_string(llpCode));
    }
    if (llpmass < pythia.particleData.m0(531) - pythia.particleData.m0(333)) {
        // Bs0
        pythia.readString("531:onMode = 3");
        pythia.readString("531:addChannel = 2 1.00000 0 333 " + to_string(llpCode));
    }
    if (llpmass < pythia.particleData.m0(533)  - pythia.particleData.m0(333)) {
        // Bsstar0
        pythia.readString("533:onMode = 3");
        pythia.readString("533:addChannel = 2 1.00000 0 333 " + to_string(llpCode));
    }
    if (llpmass < pythia.particleData.m0(5122)  - pythia.particleData.m0(3122)) {
        // Lambda_b
        pythia.readString("5122:onMode = 3");
        pythia.readString("5122:addChannel = 2 1.00000 0 3122 " + to_string(llpCode));
    }
}

void initMainPythia(Pythia8::Pythia& session, const ModelParams& model, const RunParams& run) {
    session.particleData.addParticle(model.id, "LLP", 1, 0, 0, model.mass, 0., 0., 0., model.cTauCm * 1.e3);
    session.readString(to_string(model.id) + ":oneChannel = 1 1.00000 0 211 -211");
    session.readString(to_string(model.id) + ":mayDecay = on");
    initBDecays(session, model.id, model.mass);
    session.particleData.listFF(run.path + run.baseName + "_decay_table.dat");

    session.readString("Next:numberShowEvent = 0");
    session.readString("Next:numberCount = " + to_string(run.nEventTried/100));
    session.readString("Tune:pp = " + to_string(run.tune));
    session.readString("Random:setSeed = on");
    session.readString("Random:seed = " + to_string(run.seed));

    session.settings.parm("Beams:eCM", run.sqrtS);
    session.readString("Beams:idB = 2212");
    session.readString("Beams:idA = 2212");

}

size_t initSoftQCD(Pythia8::Pythia& pythia) {
    // Generate minimum-bias events, with or without double diffraction.
    pythia.readString("SoftQCD:nonDiffractive = on");
    pythia.readString("HardQCD:all = off");
    return 1;
}

size_t initHardQCD(Pythia& pythia) {
    // Generate QCD jet events, above some threshold.
    pythia.readString("HardQCD:all = on");
    pythia.readString("SoftQCD:nonDiffractive = off");
    pythia.readString("PhaseSpace:pTHatMin = 20.");
    pythia.readString("PhaseSpace:bias2Selection = on");
    pythia.readString("PhaseSpace:bias2SelectionPow = 5.4");
    pythia.readString("PhaseSpace:bias2SelectionRef = 20.");
    return 3;
}

size_t initHeavyFlavor(Pythia8::Pythia& pythia) {
    // Generate QCD jet events, above some threshold. a subset of initHardQCD
    pythia.readString("HardQCD:all = off");
    pythia.readString("SoftQCD:nonDiffractive = off");
    // pythia.readString("HardQCD:hardccbar = on");
    pythia.readString("HardQCD:hardbbbar = on");
    pythia.readString("PhaseSpace:pTHatMin = 20.");
    pythia.readString("PhaseSpace:bias2Selection = on");
    pythia.readString("PhaseSpace:bias2SelectionPow = 5.4");
    pythia.readString("PhaseSpace:bias2SelectionRef = 20.");
    return 3;
}

bool passGeometry(const Pythia8::Vec4& x, const Pythia8::Vec4& p, const ExpParams& options) { // x is decay vertex and p is momentum
    if(options.minELLP > 0. && p.e() < options.minELLP) return false;
    double x2 = x.pAbs2();
    double r2Max = options.maxRCut * options.maxRCut * 1.e6;
    double r2Min = options.minRCut * options.minRCut * 1.e6;
    if(x2 > r2Max || x2 < r2Min) return false;
    double eta = fabs(x.eta());
    if( eta < options.absEtaMin || eta > options.absEtaMax) return false;
    return true;
}


int main(int argc, char *argv[]) {

    ModelParams llpParams;
    ExpParams kinematicsParams;
    RunParams runOptions;
    double sigma = 0.5;

    for(int i = 1; i< argc; ++i) {
        if(ALP::parseArg(argv[i], "mass", &llpParams.mass)) continue;
        if(ALP::parseArg(argv[i], "inclBRB", &llpParams.inclBRB)) continue;
        if(ALP::parseArg(argv[i], "cTauCM", &llpParams.cTauCm)) continue;
        if(ALP::parseArg(argv[i], "numEvents", &runOptions.nEventDecayed)) continue;
        if(ALP::parseArg(argv[i], "numTries", &runOptions.nEventTried)) continue;
        if(ALP::parseArg(argv[i], "sqrtS", &runOptions.sqrtS)) continue;
        if(ALP::parseArg(argv[i], "tune", &runOptions.tune)) continue;
        if(ALP::parseArg(argv[i], "seed", &runOptions.seed)) continue;
        if(ALP::parseArg(argv[i], "baseName", &runOptions.baseName)) continue;
        if(ALP::parseArg(argv[i], "absEtaMin", &kinematicsParams.absEtaMin)) continue;
        if(ALP::parseArg(argv[i], "absEtaMax", &kinematicsParams.absEtaMax)) continue;
        if(ALP::parseArg(argv[i], "minRCut", &kinematicsParams.minRCut)) continue;
        if(ALP::parseArg(argv[i], "maxRCut", &kinematicsParams.maxRCut)) continue;
        if(ALP::parseArg(argv[i], "minELLP", &kinematicsParams.minELLP)) continue;
        if(ALP::parseArg(argv[i], "sigma", &sigma)) continue;
    }
    runOptions.nEventTried = max(runOptions.nEventTried, 10*runOptions.nEventDecayed);
    double sigma2 = sigma * sigma;

    // Generator.
#ifndef HAVE_PY83
    HepMC::Pythia8ToHepMC toHepMC;
#else
    HepMC::I_Pythia8 toHepMC;
#endif
    HepMC::IO_GenEvent output_hepmc(runOptions.path + runOptions.baseName + ".hepmc", std::ios::out);

    Pythia pythia{PYTHIAXML_PATH};
    Event& event = pythia.event;
    initMainPythia(pythia, llpParams, runOptions);

    int countBad = 0;
    Hist pTw("pTHat distribution", 100, 0., 1000.);
    Hist pTw2("sigma of pTHat distribution", 100, 0., 1000.);

    double wgtBdecay = llpParams.inclBRB;


    size_t maxRun = 3;
    // for(size_t iRun = 0; iRun < maxRun; ++iRun) {
    for(size_t iRun = 2; iRun < maxRun; ++iRun) {
        Hist tmpw("pTHat distribution", 100, 0., 1000.);
        Hist tmpw2("sigma of pTHat distribution", 100, 0., 1000.);

        size_t eventsInRun = runOptions.nEventTried;

        size_t eventsWritten = 0;
        cout<<"iRun:"<<iRun<<endl;
        switch(iRun) {
        case 0:
            eventsInRun *= initSoftQCD(pythia);
            break;
        case 1:
            eventsInRun *= initHardQCD(pythia);
            break;
        case 2:
            eventsInRun *= initHeavyFlavor(pythia);
            break;
        }

        pythia.init();

        // Begin event loop.
        for (size_t iEvent = 0; iEvent < eventsInRun && eventsWritten < runOptions.nEventDecayed; ++iEvent) {

            // Generate events.  Skip if error.
            if (!pythia.next()) continue;

            double pTHat = pythia.info.pTHat();
            if (iRun == 0 && pythia.info.isNonDiffractive() && pTHat > 20.) continue;

            double baseWeight = pythia.info.weight();
            tmpw.fill(pTHat, baseWeight);
            tmpw2.fill(pTHat, baseWeight * baseWeight);

	    // this requires new syntax, but I'm not sure what it is (because pythia.org docs are down)
            // pythia.info.updateWeight(wgtBdecay * baseWeight);
            size_t found = 0;
            for (int i = 0; i < event.size(); ++i) { // find LLP that decays in the detector volume?
                if (event[i].iBotCopy() == i) {
                    auto &cand = event[i];
                    if (cand.id() == llpParams.id) {
                        if (cand.statusAbs() > 90 && cand.statusAbs() < 100) { //seems like the LLP always have status 91
                            if(passGeometry(cand.vDec(), cand.p(), kinematicsParams)) {
                                ++found;
                            }
                        }
                    }
                }
            }
            if(found > 0) {
                auto evt = unique_ptr<HepMC::GenEvent>{new HepMC::GenEvent{}};
                toHepMC.fill_next_event( pythia, evt.get() );

                // Write the HepMC event to file. Done with it.
                output_hepmc.write_event(evt.get());
                ++eventsWritten;
                // if (eventsWritten%10)cout<<"eventsWritten: "<<eventsWritten<<endl;
            }
            if(found > 1) {
                ++countBad;
            }
        }

        // Statistics.
        pythia.readString("Stat:showPartonLevel = on");
        pythia.stat();

        double sigmaNorm = pythia.info.sigmaGen() / pythia.info.weightSum();
        cout << "Scale Factor: " + to_string(sigmaNorm) << endl << "From sigma generated: " << to_string(pythia.info.sigmaGen()) << "mb  and sum of weights generated: " << to_string(pythia.info.weightSum()) << endl;
        cout << "# of events with multiple LLP in acceptance in " << to_string(2.0*runOptions.nEventTried/1000) << "k events tried and " <<  to_string(eventsWritten/1000.0) << "k written: " <<  to_string(countBad) << endl;
        pTw += sigmaNorm * tmpw;
        pTw2 += sigmaNorm * sigmaNorm * tmpw2;
    }

    pTw2.takeSqrt();
    auto relErrors = pTw2 / pTw;


    cout << relErrors;
    return 0;
}
