#include "VirtualBox.h"

const std::string SubutaiLauncher::VirtualBox::BIN = "vboxmanage";
std::string SubutaiLauncher::VirtualBox::cloneName = "subutai";
std::string SubutaiLauncher::VirtualBox::subutaiBranch = "subutai-dev";


SubutaiLauncher::VirtualBox::VirtualBox()
{
     _version = ""; 
}

SubutaiLauncher::VirtualBox::~VirtualBox() 
{

}

std::vector<SubutaiLauncher::SubutaiVM> SubutaiLauncher::VirtualBox::getPeers() 
{
    std::vector<std::string> args;
    args.push_back("list");
    args.push_back("vms");
    SubutaiProcess p;
    p.launch(BIN, args, _location);
    p.wait();
    auto out = p.getOutputBuffer();
    std::vector<SubutaiVM> peers = VirtualBox::parseVms(out);
    return peers;
}

bool SubutaiLauncher::VirtualBox::findInstallation()
{
    auto env = new Environment();
    SubutaiString str(env->getVar("PATH", ""));
    std::vector<std::string> path;
    str.split(':', path);
    FileSystem fs;
    for (auto it = path.begin(); it != path.end(); it++) {
    	fs.setPath((*it));
	if (fs.isFileExists(BIN)) {
	    _installed = true;
	    _path = (*it);
	    _location = _path;
	    _path.append(FileSystem::DELIM);
	    _path.append(BIN);
	    return true;
	}
    }
    return false;
}


bool SubutaiLauncher::VirtualBox::isInstalled() 
{
    return _installed;
}

bool SubutaiLauncher::VirtualBox::isRunning() 
{
    return true;
}

bool SubutaiLauncher::VirtualBox::isUpdateRequired() 
{
    return false;
}

std::string SubutaiLauncher::VirtualBox::extractVersion()
{
    if (_version != "") {
        return _version;
    }

    std::vector<std::string> args;
    args.push_back("-v");

    SubutaiProcess p;
    p.launch(BIN, args, _location);
    if (p.wait() == 0) {
        _version = p.getOutputBuffer();
        return _version;
    }
    return "";
}

void SubutaiLauncher::VirtualBox::getVms()
{
    std::vector<std::string> args;
    args.push_back("list");
    args.push_back("vms");
    SubutaiProcess p;
    p.launch(BIN, args, _location);
    p.wait();
    auto out = p.getOutputBuffer();
}

std::vector<SubutaiLauncher::SubutaiVM> SubutaiLauncher::VirtualBox::parseVms(const std::string& buffer)
{
    char vmname[150];
    char vmid[150];
    int bsize = 256;
    // Parsing output of vboxmanage:
    // "machine_name" {machine_id}
    std::vector<SubutaiVM> vms;
    SubutaiString buf(buffer);
    std::vector<std::string> lines;
    buf.split('\n', lines);
    for (auto it = lines.begin(); it != lines.end(); it++) {
	if ((*it).empty() || (*it).length() < 10) continue;
	    const char* line = const_cast<char*>((*it).c_str());
#if LAUNCHER_LINUX
	    sscanf(line, "\"%[^\"]\" {%s}", vmname, vmid);
#elif LAUNCHER_WINDOWS
	    sscanf_s(line, "\"%s\" {%s}", vmname, bsize, vmid, bsize);
#elif LAUNCHER_MACOS
#error Not Implemented on this platform
#endif
	SubutaiVM v;
	v.name = std::string(vmname);
	v.id = std::string(vmid);
	int i = v.name.find("subutai",0);
	if (i != std::string::npos){
	    vms.push_back(v);
	}
    }
    return vms;
}

std::string SubutaiLauncher::VirtualBox::execute(const std::string& command)
{
    SubutaiString str(command);
    std::vector<std::string> args;
    str.split(' ', args);
    SubutaiProcess p;
    p.launch(BIN, args, _location);
    p.wait();
    std::string out = p.getOutputBuffer();
    Log::instance()->logger()->debug() << "VirtualBox::execute " << command << ": "<< out << std::endl;
    int i = 0;
    return out;
}

std::string SubutaiLauncher::VirtualBox::sysExecute(const std::string& command, const std::string& cargs)
{
    SubutaiString str(cargs);
    std::vector<std::string> args;
    str.split(' ', args);
    SubutaiProcess p;
    if (p.launch(command, args, "/usr/bin") < 0) {
	return "Error: can not start process";
    }
    if (p.wait() < 0) {
	return "Error: can not wait process";;
    }
    std::string out = p.getOutputBuffer();
    Log::instance()->logger()->debug() << "VirtualBox::sysExecute " << command << " args: " << cargs << " output: " << out << std::endl;
    return out;
}

std::string SubutaiLauncher::VirtualBox::sysExecute(const std::string& command, const std::vector<std::string>& args)
{
    SubutaiProcess p;
    if (p.launch(command, args, "/usr/bin") < 0) {
	return "Error: can not start process";
    }
    if (p.wait() < 0) {
	return "Error: can not wait process";
    }
    std::string out = p.getOutputBuffer();
    Log::instance()->logger()->debug() << "VirtualBox::sysExecute " << command << " output: " << out << std::endl;
    return out;
}

std::string SubutaiLauncher::VirtualBox::cloneVM(){

    std::string ovaName = "core";
    Log::instance()->logger()->debug() << "VirtualBox::cloneVM start" << std::endl;
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y%m%d%H%M");
    auto strTime = oss.str();

    cloneName.append("-");
    cloneName.append(strTime);
    Log::instance()->logger()->debug() << "VirtualBox::cloneVM " << cloneName << std::endl;

    execute("natnetwork add --netname natnet1 --network '10.0.5.0/24' --enable --dhcp on");
    execute("import /tmp/subutai/core.ova");


    execute("clonevm --register --name " + cloneName + " core");
    execute("modifyvm " + cloneName  + " --nic1 none");
    execute("modifyvm " + cloneName  + " --nic2 none");
    execute("modifyvm " + cloneName  + " --nic3 none");
    execute("modifyvm " + cloneName  + " --nic4 nat");
    execute("modifyvm " + cloneName  + " --cableconnected4 on");
    execute("modifyvm " + cloneName  + " --natpf4 \"ssh-fwd,tcp,,5567,,22\"");
    execute("modifyvm " + cloneName  + " --memory 2048");
    execute("modifyvm " + cloneName  + " --cpus 2");
    execute("modifyvm " + cloneName  + " --rtcuseutc on");
    execute("startvm " + cloneName );
    execute("unregistervm --delete " + ovaName);

    return cloneName;
}

bool SubutaiLauncher::VirtualBox::cleanKnownHosts(std::string sport){

    Environment env;
    std::string khDir = env.getVar("HOME","~");
    Log::instance()->logger()->debug() << "VirtualBox::cleanKnownHosts home dir " << khDir << std::endl;

    std::string cmd = "-f ";
    cmd.append(khDir);
    Log::instance()->logger()->debug() << "VirtualBox::cleanKnownHosts home dir " << cmd << std::endl;
    std::string cmd1 = cmd;
    cmd.append("/.ssh/known_hosts -R [localhost]:");
    cmd.append(sport);
    Log::instance()->logger()->debug() << "VirtualBox::cleanKnownHosts cmd /" << cmd << "/" << std::endl;

    cmd1.append("/.ssh/known_hosts -R [127.0.0.1]:");
    cmd1.append(sport);
    Log::instance()->logger()->debug() << "VirtualBox::cleanKnownHosts cmd1 /" << cmd1 << "/" << std::endl;
    std::string out = sysExecute("ssh-keygen", cmd);
    Log::instance()->logger()->debug() << "VirtualBox::cleanKnownHosts " << " cmd " << cmd << " out: " << out << std::endl;
    out = sysExecute("ssh-keygen", cmd1);
    Log::instance()->logger()->debug() << "VirtualBox::cleanKnownHosts " << " cmd " << cmd1 << " out: " << out << std::endl;
    return true;
}


bool SubutaiLauncher::VirtualBox::runScripts(std::string instVersion)
{
    sleep(20);
    //waiting for ssh - waitForSnapd
    SSH ssh;
    ssh.setHost("localhost", 5567);
    ssh.setUsername("subutai", "ubuntai");

    auto l =  Log::instance()->logger();

    cleanKnownHosts("5567");
    if (!waitingSSH(ssh, "5567")) {
	//try to restart VM and try once more
	execute("restartvm " + cloneName );
	sleep(20);
	if (!waitingSSH(ssh, "5567")) {
	    l->error() << "VirtualBox::runScript can not connect to  " << cloneName << ", check VM status " << ssh.isConnected() << std::endl;
	    throw SubutaiException("Can not connect to VM, probqably VM can not start", 15);
	    return false;
	}
    }
;
    connectSSH(ssh);
    l->debug() << "VirtualBox::runScript cloneName " << cloneName << " connected: " << ssh.isConnected() << std::endl;

    //SUBUTAI - get branch
    std::string subutaiBranch = getBranch(instVersion);

    //Install snap from store

    std::string cmdArgsExt = "sudo snap install --beta --devmode ";
    cmdArgsExt.append(subutaiBranch);

    l->debug() << "VirtualBox::runScript Install snap from store: " << cmdArgsExt << std::endl;
    //sout = ssh.execute("sudo snap install --beta --devmode " + subutaiBranch);
    std::string sout = ssh.execute(cmdArgsExt);
    l->debug() << "VirtualBox::runScript After Install snap from store: " << sout << std::endl;
    sleep(20);

    //waitForSubutai    -    wait for subutai installation complete
    sout = "1";
    l->debug() << "VirtualBox::runScript waitForSubutai " << std::endl;
    long found = std::string::npos;
    std::string sshCommand = "bash -c 'sudo snap list ";
    sshCommand.append(subutaiBranch);
    sshCommand.append("'");
    sout = ssh.execute(sshCommand);
    l->debug() << "VirtualBox::runScript waitForSubutai command: " << sshCommand << "sout = /" << sout << "/"<<std::endl;
    
    //while (sout != "0") {
/*
    while (found == std::string::npos){
        //sout = ssh.execute(sshCommand);
        sout = ssh.execute(sshCommand);
	found = sout.find(subutaiBranch,0);
	l->debug() << "VirtualBox::runScript waitForSubutai " << subutaiBranch << "  sout = " << sout << " found = " << found << std::endl;
	sleep(10);
    } 
*/

    sleep (20);
    sout = ssh.execute("sudo install -D /dev/null /writable/system-data/var/lib/console-conf/complete");
    l->debug() << "VirtualBox::runScript sudo install -D /dev/null /" << sout << "/" <<std::endl;
    sleep (20);

    //Set Alias
    sshCommand = "sudo bash -c 'snap alias ";
    sshCommand.append(subutaiBranch);
    sshCommand.append(" subutai'");
    sout = ssh.execute(sshCommand);
    l->debug() << "VirtualBox::runScript Set Alias: " << sshCommand << " sout = /" << sout << "/" <<std::endl;
    sleep (20); 

    //btrfs init
    //Initializing Btrfs disk
    sshCommand = "sudo ";
    sshCommand.append(subutaiBranch);
    sshCommand.append(".btrfsinit /dev/sdb");
    sout = ssh.execute(sshCommand);
    l->debug() << "VirtualBox::runScript Btrfs disk init: " << sshCommand << " out = /" << sout << "/" <<std::endl;
    //ssh.execute("sudo " + subutaiBranch + ".btrfsinit /dev/sdb");
    sleep (20); 
    std::string out = "";

    //addSSHkey
    //Adding user key to peer
    out = sysExecute("ssh-add", "-L");
    l->debug() << "VirtualBox::runScript ssh-add after: "<< out << std::endl;
    if (out != "") {
        sshCommand = "sudo bash -c 'echo ";
	sshCommand.append(out);
	sshCommand.append(">>/root/.ssh/authorized_keys'");
	sout = ssh.execute(sshCommand);
	l->debug() << "VirtualBox::runScript adding key: " << sshCommand << " out = /" << sout << "/" <<std::endl;
	//"sudo bash -c 'echo " + out + " >> /root/.ssh/authorized_keys'"
    }
    cleanKnownHosts("5567");
    l->debug() << "VirtualBox::runScript ssh-keygen after: "<< out << std::endl;

    //setAutobuildIP
    setAutobuildIP(ssh);

    //stopVM
    l->debug() << "VirtualBox::runScript sync before stop: " << " out = /" << "/" <<std::endl;
    //sout = 
    //ssh.execute("sudo sync");
    sleep(10);
    l->debug() << "VirtualBox::runScript after stop: " << " out = /" << "/" <<std::endl;
    ssh.disconnect();
    stopVM();

    restoreNet();

    //startVM
    //"Starting vm"
    startVM();

/*    if (!startVM()) {
	l->error() << "VirtualBox::runScript start error: " << " out = /" << "/" <<std::endl;
	throw SubutaiException("Can not establish ssh session to VM");
	return false;
    };
*/

    //if MH!
    //importManagement();
    return true;
}

bool SubutaiLauncher::VirtualBox::importManagement(){

    auto l = Log::instance()->logger();
    SSH ssh1;
    ssh1.setUsername("subutai", "ubuntai");
    ssh1.setHost("localhost", 4567);
    cleanKnownHosts("4567");

    if (!waitingSSH(ssh1, "4567")) {
	//try to restrt VM and try once more
	execute("restartvm " + cloneName );
	sleep(20);
	if (!waitingSSH(ssh1, "4567")) {
	    l->error() << "VirtualBox::runScript can not connect to  " << cloneName << ", check VM status " << ssh1.isConnected() << std::endl;
	    throw SubutaiException("Can not connect to VM, probqably VM can not start", 15);
	    return false;
	}
    }    

    connectSSH(ssh1);

    //waitPeerIP
    //waitPeerIP();

    std::string sshCommand = "sudo ";
    sshCommand.append(subutaiBranch);
    sshCommand.append(" -d import management 2>&1 > management_log");
    //sout = 
    ssh1.execute(sshCommand);
    l->debug() << "VirtualBox::runScript import management: " << sshCommand << std::endl;
    sleep (20); 
    //ssh1.disconnect();
    return true;
}



bool SubutaiLauncher::VirtualBox::runAutobuild(){
    std::string out = sysExecute("/tmp/subutai/autobuild.sh", "");
    Log::instance()->logger()->debug() << "VirtualBox::runAutobuild" << std::endl;
    return true;
}

std::string SubutaiLauncher::VirtualBox::getBranch(std::string instVersion) {

    if (instVersion == "STAGE") {
	return "subutai-master";
    } else if (instVersion == "DEV") {
	return "subutai-dev";
    }
    return "subutai";
}


bool SubutaiLauncher::VirtualBox::stopVM() {
    //sout = ssh.execute("sudo sync;sync");
    execute("controlvm " + cloneName + " poweroff");
    Log::instance()->logger()->debug() << "VirtualBox::stopVM power off after" << std::endl;
    return true;
}

bool SubutaiLauncher::VirtualBox::startVM() {
    //"Starting vm"
    //std::string vout = execute("startvm --type headless " + cloneName);
    std::string vout = execute("startvm  " + cloneName);
    Log::instance()->logger()->debug() << "VirtualBox::startVM" << cloneName << " out: " << vout <<  std::endl;;
    return true;
}

bool SubutaiLauncher::VirtualBox::waitingSSH(SSH &s_ssh, std::string sport) {
    //Check when can ssh

    auto l =  Log::instance()->logger();
    l->debug() << "VirtualBox::waitingSSH " << cloneName << " port: /" << sport << "/" << std::endl;

    std::string cmdArgsInt; 
    std::string cmdArgsExt; 

    std::string sout = "1";
    std::string out = "1";
    int i = 0;

    std::vector<std::string> args;
    args.push_back("-c");
    cmdArgsInt = "sshpass -p ubuntai ssh -o IdentitiesOnly=yes -o ConnectTimeout=1 -o StrictHostKeyChecking=no subutai@localhost -p";
    cmdArgsInt.append(sport);
    cmdArgsInt.append(" ls > /dev/null 2>&1;echo $?");
    //args.push_back("sshpass -p ubuntai ssh -o IdentitiesOnly=yes -o ConnectTimeout=1 -o StrictHostKeyChecking=no subutai@localhost -p5567 ls > /dev/null 2>&1;echo $?");
    args.push_back(cmdArgsInt);

    out = sysExecute("bash", args);
    l->debug() << "VirtualBox::waitingSSH test ssh " << out  << std::endl;

    int c = out.find("0", 0);
    while (c == std::string::npos){
	if (i < 15 ){
	    out = sysExecute("bash", args);
	    c = out.find("0", 0);
    	    l->debug() << "VirtualBox::waitingSSH: /"  << "/ out: " << out << std::endl;
    	    sleep(10);
	    i++;
	}
    }
    if (c == std::string::npos){
	return false;
    }

    while (!s_ssh.isConnected()){
        l->debug() << "VirtualBox::runScript before connect " << s_ssh.isConnected() << std::endl;
	s_ssh.connect();
	sleep(10);
    }
    //sleep(30);
    l->debug() << "VirtualBox::startVM after" << s_ssh.isConnected() << std::endl;
    return true;
}

bool SubutaiLauncher::VirtualBox::connectSSH(SSH &s_ssh) {
    //Connecting and authenticate 
    auto l = Log::instance()->logger();

    while (!s_ssh.isConnected()){
        l->debug() << "VirtualBox::connectSSH before connect " << s_ssh.isConnected() << std::endl;
	s_ssh.connect();
	sleep(10);
    }

    l->debug() << "VirtualBox::connectSSH after connect " << s_ssh.isConnected() << std::endl;
    s_ssh.verifyHost();
    l->debug() << "VirtualBox::connectSSH after host verified " << s_ssh.isConnected() << std::endl;
    s_ssh.authenticate();
    l->debug() << "VirtualBox::connectSSH after host verified first authenticated " << s_ssh.isConnected() << std::endl;

    if (!s_ssh.isAuthenticated()){
	l->debug() << "VirtualBox::connectSSH before authenticate " << s_ssh.isAuthenticated() << std::endl;
	s_ssh.authenticate();
        l->debug() << "VirtualBox::connectSSH after authenticate " << s_ssh.isAuthenticated() << std::endl;
    } else 
	return true;
    while (!s_ssh.isAuthenticated()){
	l->debug() << "VirtualBox::connectSSH before authenticate " << s_ssh.isAuthenticated() << std::endl;
	s_ssh.authenticate();
	sleep(10);
        l->debug() << "VirtualBox::connectSSH after authenticate " << s_ssh.isAuthenticated() << std::endl;
    }
    return true;
}

bool SubutaiLauncher::VirtualBox::restoreNet(){
    //restoreNet
    //Restoring network
    sleep(20);
    std::string vout;

    std::string out = "1";
    auto l = Log::instance()->logger();
    l->debug() << "VirtualBox::restoreNet" << std::endl;

    std::vector<std::string> args;
    args.push_back("-c");
    args.push_back("vboxmanage list hostonlyifs | grep -c vboxnet0");

    out = sysExecute("bash", args);
    l->debug() << "VirtualBox::restoreNet : vboxnet0" << out << std::endl;
    if (out == "0"){
	vout = execute("hostonlyif create");
	l->debug() << "VirtualBox::restoreNet hostonlyif create: " << vout << std::endl;
    }

    vout = execute("hostonlyif ipconfig vboxnet0 --ip 192.168.56.1");
    l->debug() << "VirtualBox::restoreNet ipconfig vboxnet0 --ip 192.168.56.1 : " << vout << std::endl;

    args.clear();
    args.push_back("-c");
    args.push_back("vboxmanage list dhcpservers | grep -c vboxnet0");

    out = sysExecute("bash", args);
    l->debug() << "VirtualBox::restoreNet  list dhcpservers: " << out << std::endl;
    if (out == "0"){
	vout == execute("dhcpserver add --ifname vboxnet0 --ip 192.168.56.1 --netmask 255.255.255.0 --lowerip 192.168.56.100 --upperip 192.168.56.200");
	l->debug() << "VirtualBox::restoreNet dhcpserver add --ifname vboxnet0 --ip 192.168.56.1  : " << vout << std::endl;
    }

    vout = execute("dhcpserver modify --ifname vboxnet0 --enable");
    l->debug() << "VirtualBox::restoreNet  modify --ifname vboxnet0 --enable: " << vout << std::endl;

    vout = execute("modifyvm " + cloneName + " --nic4 none");
    l->debug() << "VirtualBox::restoreNet  --nic4 none: " << vout << std::endl;

    vout = execute("modifyvm " + cloneName + " --nic3 hostonly");
    l->debug() << "VirtualBox::restoreNet  --nic3 hostonly: " << vout << std::endl;

    vout = execute("modifyvm " + cloneName + " --hostonlyadapter3 vboxnet0");
    l->debug() << "VirtualBox::restoreNet  --hostonlyadapter3 vboxnet0: " << vout << std::endl;

    vout = execute("modifyvm " + cloneName + " --nic2 nat");
    l->debug() << "VirtualBox::restoreNet  : --nic2 nat" << vout << std::endl;

    vout = execute("modifyvm " + cloneName + " --natpf2 \"ssh-fwd,tcp,,4567,,22\"");//!!!!!!!!!!!!!!
    l->debug() << "VirtualBox::restoreNet  : --natpf2 \"ssh-fwd,tcp,,4567,,22\"" << vout << std::endl;

    vout = execute("modifyvm " + cloneName + " --nic1 bridged");
    l->debug() << "VirtualBox::restoreNet  --nic1 bridged: " << vout << std::endl;

    args.clear();
    args.push_back("-c");

    args.push_back("iface=$(/sbin/route -n|grep ^0.0.0.0);iface=${iface/\t/ /};iface=$(echo $iface|cut -d\" \" -f8);echo $iface");

    std::string iface = sysExecute("bash", args);
    SubutaiString st(iface);
    iface = st.remove("\n","");
    l->debug() << "VirtualBox::restorenet  default gw interface: /" << iface << "/" << std::endl;

    vout = execute("modifyvm " + cloneName + " --bridgeadapter1 " + iface);
    l->debug() << "VirtualBox::restoreNet  --bridgeadapter1  " << vout << std::endl;
    return true;
}


bool SubutaiLauncher::VirtualBox::setAutobuildIP(SSH& s_ssh) {

    auto l = Log::instance()->logger();
    l->debug() << "VirtualBox::AutoBuildIP: start " << std::endl;

    std::vector<std::string> args;
    args.push_back("-c");
    //args.push_back("ip=$(/sbin/route -n | grep ^0.0.0.0 | awk '{print $8}');echo $ip");
    args.push_back("iface=$(/sbin/route -n|grep ^0.0.0.0);iface=${iface/\t/ /};iface=$(echo $iface|cut -d\" \" -f8);echo $iface");

    std::string iface = sysExecute("bash", args);
    SubutaiString st(iface);
    iface = st.remove("\n","");
    l->debug() << "VirtualBox::setAutobuildIP()  default gw interface: /" << iface << "/" << std::endl;

    args.clear();

    //Setting loopback ip - address of default gateway
    args.push_back("-c");
    std::string cmdInt = "/bin/ip addr show ";
    cmdInt.append(iface);
    cmdInt.append("|grep -Po \'inet \\K[\\d.]+\'");
    args.push_back(cmdInt);

    //std::string out = sysExecute("bash", "-c \"ip=$(/bin/ip addr show `/sbin/route -n | grep ^0.0.0.0 | awk '{print $8}'` | grep -Po 'inet \\K[\\d.]+')\"");
    //out = sysExecute("bash", "-c '/bin/ip addr show `/sbin/route -n | grep ^0.0.0.0 | awk '{print $8}'` | grep -Po 'inet \\K[\\d.]'");
    std::string out = sysExecute("bash", args);
    //ip = out;
    st = SubutaiString(out);
    out = st.remove("\n","");
    l->debug() << "VirtualBox::AutoBuildIP: gw address: " <<  " out = /" << out << "/" <<std::endl;

    std::string sshCommand = "sudo bash -c 'echo ";
    sshCommand.append(out);
    sshCommand.append(" > /var/snap/");
    sshCommand.append(subutaiBranch);
    sshCommand.append("/current/.ip'");
    //std::string sout = 
    s_ssh.execute(sshCommand);
    //l->debug() << "VirtualBox::runScript set AutoBuildIp: " << sshCommand << " out = /"  << "/" <<std::endl;
    l->debug() << "VirtualBox::runScript set AutoBuildIp:  current/.ip" <<std::endl;
    sleep (10); 

    //s_ssh.execute("sudo bash -c 'echo $ip > /var/snap/" + subutaiBranch + "/current/.ip'");
    return true;
}

bool SubutaiLauncher::VirtualBox::waitPeerIP() {
    //   echo "Waiting for Subutai IP address"
    auto l = Log::instance()->logger();
    l->debug() << "VirtualBox::waitPeerIP() starting: /" << "/" << std::endl;

    std::vector<std::string> args;
    args.push_back("-c");
    args.push_back("ip=$(nc -l 48723);echo $ip");

    std::string ip = sysExecute("bash", args);
    SubutaiString st(ip);
    ip = st.remove("\n","");
    l->debug() << "VirtualBox::waitPeerIP() ip: /" << ip << "/" << std::endl;

    Environment env;
    std::string khDir = env.getVar("HOME","~");
    Log::instance()->logger()->debug() << "VirtualBox::cloneVM home dir " << khDir << std::endl;


    std::string argInt = "-f ";
    argInt.append(khDir);
    argInt.append("/.ssh/known_hosts -R ");
    argInt.append(ip);

    //std::string out = sysExecute("ssh-keygen", "-f ~/.ssh/known_hosts -R $ip > /dev/null 2>&1");
    std::string out = sysExecute("ssh-keygen", argInt);
    l->debug() << "VirtualBox::waitPeerIP() ssh-keygen " << argInt << "/" << out << "/" << std::endl;
    //timeout 300 echo -e "*******\\nPlease use following command to access your resource host:\\nssh root@$ip\\nor login \"subutai\" with password \"ubuntai\"\\n*******"
    return true;
}
