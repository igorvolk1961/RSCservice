#ifndef SVGDESC_H
#define SVGDESC_H

class QDomElement;
class SvgDescPrivate;

class SvgDesc{
public:
  SvgDesc(QDomElement* svg_node, QDomElement* g_node, QDomElement* desc_node);
  ~SvgDesc();

  QDomElement* appendImg(const char* type);
  QDomElement* appendSection();
  void leaveCurrentNode();
  QDomElement* getSvg_node();
  QDomElement* getG_node();
  QDomElement* getCurrent_node();
  int getCurrent_id();
private:
  QDomElement* svg_node;
  QDomElement* g_node;
  QDomElement* desc_node;
  SvgDescPrivate* d;
  int current_id;
  int prepareImageLevel;
};

#endif // SVGDESC_H
