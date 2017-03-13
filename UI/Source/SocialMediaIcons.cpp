/*
  ==============================================================================

    SocialMediaIcons.cpp
    Created: 23 Aug 2016 7:32:40pm
    Author:  crioto

  ==============================================================================
*/

#include "SocialMediaIcons.h"

SocialMediaIcons::SocialMediaIcons() {
    _facebook = new DrawableButton("Facebook", DrawableButton::ImageFitted);
    _twitter = new DrawableButton("Twitter", DrawableButton::ImageFitted);
    _linkedin = new DrawableButton("LinkedIn", DrawableButton::ImageFitted);
    _github = new DrawableButton("GitHub", DrawableButton::ImageFitted);

    _facebook->addListener(this);
    _twitter->addListener(this);
    _linkedin->addListener(this);
    _github->addListener(this);

    DrawablePath fb, fbHover;
    DrawablePath twitter, twitterHover;
    DrawablePath linkedin, linkedinHover;
    DrawablePath github, githubHover;

    FillType ft(Colour(0xffffffff));
    FillType fth(Colour(0x34557c));

    fb.setPath(getFacebook());
    fb.setFill(ft);
    fbHover.setPath(getFacebook());
    fbHover.setFill(fth);


    twitter.setPath(getTwitter());
    twitter.setFill(ft);

    linkedin.setPath(getLinkedin());
    linkedin.setFill(ft);

    github.setPath(getGithub());
    github.setFill(ft);

    _facebook->setImages(&fb, &fbHover, &fbHover);
    _facebook->setBounds(55, 5, 30, 30);

    _twitter->setImages(&twitter, &twitter, 0);
    _twitter->setBounds(90, 5, 30, 30);

    _linkedin->setImages(&linkedin, &linkedin, 0);
    _linkedin->setBounds(130, 5, 30, 30);

    _github->setImages(&github, &github, 0);
    _github->setBounds(170, 5, 30, 30);

    addAndMakeVisible(_facebook);
    addAndMakeVisible(_twitter);
    addAndMakeVisible(_linkedin);
    addAndMakeVisible(_github);

    addAndMakeVisible(_facebookLink);
    addAndMakeVisible(_twitterLink);
    addAndMakeVisible(_linkedinLink);
    addAndMakeVisible(_githubLink);

    _facebookLink.setBounds(-1, -1, 1, 1);
    _facebookLink.setURL(URL("https://www.facebook.com"));
    _twitterLink.setBounds(-1, -1, 1, 1);
    _twitterLink.setURL(URL("https://www.twitter.com"));
    _linkedinLink.setBounds(-1, -1, 1, 1);
    _linkedinLink.setURL(URL("https://www.linkedin.com"));
    _githubLink.setBounds(-1, -1, 1, 1);
    _githubLink.setURL(URL("https://www.github.com"));

}

SocialMediaIcons::~SocialMediaIcons() {
    delete _facebook;
    delete _twitter;
    delete _linkedin;
    delete _github;
}

void SocialMediaIcons::buttonClicked(Button* button) {
    if (button == _facebook) {
        _facebookLink.triggerClick();
    } else if (button == _twitter) {
        _twitterLink.triggerClick();
    } else if (button == _linkedin) {
        _linkedinLink.triggerClick();
    } else if (button == _github) {
        _githubLink.triggerClick();
    }
}

void SocialMediaIcons::paint(Graphics& g) {
}

void SocialMediaIcons::resized() {

}

Path SocialMediaIcons::getFacebook() {
    return Drawable::parseSVGPath(
	"M90,15.001C90,7.119,82.884,0,75,0H15C7.116,0,0,7.119,0,15.001v59.998   C0,82.881,7.116,90,15.001,90H45V56H34V41h11v-5.844C45,25.077,52.568,16,61.875,16H74v15H61.875C60.548,31,59,32.611,59,35.024V41   h15v15H59v34h16c7.884,0,15-7.119,15-15.001V15.001z");
}

Path SocialMediaIcons::getTwitter() {
    return Drawable::parseSVGPath(
	"M67.261,67.496H45.054c-3.087,0-5.712-1.08-7.869-3.25c-2.167-2.172-3.238-4.797-3.238-7.898v-7.904H65.59   c2.854,0,5.312-1.025,7.354-3.062c2.041-2.054,3.066-4.509,3.066-7.366c0-2.867-1.025-5.319-3.072-7.366   c-2.049-2.042-4.515-3.066-7.381-3.066H33.946V11.254c0-3.09-1.102-5.735-3.29-7.939C28.478,1.107,25.842,0,22.782,0   c-3.146,0-5.825,1.091-8.004,3.25C12.591,5.416,11.5,8.084,11.5,11.264v45.089c0,9.274,3.278,17.197,9.837,23.773   C27.901,86.715,35.814,90,45.066,90h22.203c3.082,0,5.729-1.107,7.93-3.314c2.203-2.197,3.302-4.849,3.302-7.936   c0-3.088-1.099-5.734-3.302-7.941C72.997,68.607,70.347,67.496,67.261,67.496z"
            );
}

Path SocialMediaIcons::getLinkedin() {
    return Drawable::parseSVGPath(
	"M430.117,261.543V420.56h-92.188V272.193c0-37.271-13.334-62.707-46.703-62.707   c-25.473,0-40.632,17.142-47.301,33.724c-2.432,5.928-3.058,14.179-3.058,22.477V420.56h-92.219c0,0,1.242-251.285,0-277.32h92.21   v39.309c-0.187,0.294-0.43,0.611-0.606,0.896h0.606v-0.896c12.251-18.869,34.13-45.824,83.102-45.824   C384.633,136.724,430.117,176.361,430.117,261.543z M52.183,9.558C20.635,9.558,0,30.251,0,57.463   c0,26.619,20.038,47.94,50.959,47.94h0.616c32.159,0,52.159-21.317,52.159-47.94C103.128,30.251,83.734,9.558,52.183,9.558z    M5.477,420.56h92.184v-277.32H5.477V420.56z"
            );
}

Path SocialMediaIcons::getGithub() {
    return Drawable::parseSVGPath(
	"M65.709,10.5c1.17,2.657,1.928,7.07,0.797,10.301c4.625,3.949,6.215,13.549,3.982,21.394   C77.08,42.698,84.932,41.953,90,43.976c-4.738-0.609-11.209-1.804-17.32-1.388c-1.158,0.079-2.873-0.044-2.789,1.585   c7.074,0.555,14.104,1.16,19.912,2.973c-5.761-0.941-12.754-2.375-19.912-2.574c-2.957,5.835-8.908,8.703-17.123,9.307   c0.883,1.922,2.574,2.653,2.987,5.744c0.616,4.618-0.964,11.381,0.595,14.459c0.748,1.475,1.967,1.514,2.789,2.775   c-2.012,2.381-7-0.268-7.568-2.775c-0.973-4.295,1.482-10.953-1.192-13.865c0.188,4.685-1.108,11.264,0.199,15.449   c0.515,1.646,2.002,2.281,1.593,3.766c-9.352,0.949-5.443-12.104-6.972-19.81c-1.411,0.101-0.791,2.113-0.796,2.972   c-0.04,7.524,1.54,17.844-6.57,16.838c-0.237-1.582,1.088-2.119,1.593-3.563c1.479-4.233-0.277-10.542,0.401-15.651   c-3.095,2.334,0.325,10.48-1.593,14.657c-1.105,2.404-4.666,3.45-7.367,2.377c0.352-1.79,2.2-1.501,2.984-3.169   c1.096-2.324,0.008-5.674,0.399-9.111c-5.729,1.144-10.173-0.166-12.348-3.764c-0.973-1.615-1.214-3.52-2.39-4.951   c-1.17-1.432-3.04-1.625-3.582-3.563c7.095-1.708,7.367,7.302,13.739,7.524c1.954,0.071,2.97-0.563,4.778-0.988   c0.506-2.272,1.589-3.967,3.186-5.153c-7.929-1.085-14.477-3.554-17.522-9.504c-7.208,0.354-13.804,1.317-19.913,2.771   c5.545-2.014,12.384-2.736,19.715-2.973c-0.432-2.596-4.219-1.882-6.57-1.782C8.973,42.781,3.473,43.324,0,43.976   c5.034-1.795,12.098-1.571,18.918-1.585c-2.086-6.54-1.363-16.929,3.186-20.798C20.815,18.344,21.215,13.03,22.9,10.5   c5.079,0.228,8.159,2.443,11.35,4.557c3.957-1.125,8.118-1.685,13.54-1.387c2.28,0.126,4.651,1.151,6.369,0.989   c1.685-0.158,3.542-2.049,5.178-2.771C61.468,10.947,63.271,10.588,65.709,10.5z"
            );
}
