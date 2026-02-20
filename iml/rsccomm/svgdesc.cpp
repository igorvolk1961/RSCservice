#include "svgdesc.h"
#include <QtXml>

class SvgDescPrivate{
public: QDomElement current_node;
};

SvgDesc::SvgDesc(QDomElement* svg_node, QDomElement* g_node, QDomElement* desc_node){
    this->svg_node = svg_node;
    this->g_node = g_node;
    this->desc_node = desc_node;
    current_id = 0;
    prepareImageLevel = 0;

    d = new SvgDescPrivate();
    d->current_node =  this->desc_node->appendChild(desc_node->ownerDocument().createElement("rsc:IMG")).toElement();
    d->current_node.setAttribute("id", current_id);
}

SvgDesc::~SvgDesc(){
    delete d;
}

QDomElement* SvgDesc::appendImg(const char* type){
    current_id++;
    QDomDocument doc = d->current_node.ownerDocument();
    d->current_node =  d->current_node.appendChild(doc.createElement("rsc:IMG")).toElement();
    d->current_node.setAttribute("type", type);
    d->current_node.setAttribute("id", current_id);
    return &d->current_node;
}

QDomElement* SvgDesc::appendSection(){
    d->current_node =  d->current_node.appendChild(d->current_node.ownerDocument().createElement("rsc:Section")).toElement();
    return &d->current_node;
}

void SvgDesc::leaveCurrentNode(){
    d->current_node = d->current_node.parentNode().toElement();
}

QDomElement* SvgDesc::getSvg_node(){
    return svg_node;
}

QDomElement* SvgDesc::getG_node(){
    return g_node;
}

QDomElement* SvgDesc::getCurrent_node(){
    return &d->current_node;
}

int SvgDesc::getCurrent_id(){
    return current_id;
}


