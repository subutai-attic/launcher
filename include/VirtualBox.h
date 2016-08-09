#ifndef __SL_VIRTUAL_BOX_H__
#define __SL_VIRTUAL_BOX_H__

#include <cstdlib>
#include <iostream>
#include <utility>
#include <locale>
#include <codecvt>

#include "Vars.h"

#include <nsString.h>

#include "Environment.h"
#include "String.h"
#include "FileSystem.h"
#include "VBox.h"

namespace SubutaiLauncher {

    template <class Facet>
        class usable_facet : public Facet {
            public:
                using Facet::Facet;
                ~usable_facet() {}
        };

    template<typename internT, typename externT, typename stateT>
        using codecvt = usable_facet<std::codecvt<internT, externT, stateT>>;

    class VirtualBox {
        public:
            static const std::string BIN;
            VirtualBox();
            ~VirtualBox();
            bool isInstalled();
            bool isRunning();
            bool isUpdateRequired();
            std::string retrieveVersion();
        protected:
            void loadMachines();

        private:
            void convertName(PRUnichar*);
            std::string _version;
            std::string _path;
            bool _installed;
            bool _running;
            bool _updateRequired;

            IVirtualBox* _vbox;

    };

};

#endif
