#include "bond.h"

Bond::Bond(Atom *atom1, Atom *atom2, DrawingInfo *info, QGraphicsItem *parent)
        :QGraphicsLineItem(parent),
        myStartAtom(atom1),
        myEndAtom(atom2),
        _info(info),
        myThickness(DEFAULT_BOND_THICKNESS),
        hoverOver(false),
        dashedLine(false),
        myLabel(0)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setAcceptsHoverEvents(true);
    updatePosition();
    // So that the width of the line is correct when determining the shape
    _info->getBondPen().setWidth(effectiveWidth);
    setPen(_info->getBondPen());
    _info->getBondPen().setWidthF(hoverOver ? 1.5*effectiveWidth : effectiveWidth);

    myLength = computeLength();
}

void Bond::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // A null event to prevent unwanted deselection
    Q_UNUSED(event);
}

void Bond::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // A null event to prevent unwanted deselection
    Q_UNUSED(event);
}

void Bond::toggleLabel()
{
    if(myLabel == 0){
        myLabel = new Label(Label::BondLabelType, myLength, _info);
        // Time to figure out how to make the bond labels appear correctly
        updatePosition();
    }else{
        delete myLabel;
        myLabel = 0;
    }
}

void Bond::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    hoverOver = true;
    update();
}

void Bond::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    hoverOver = false;
    update();
}

void Bond::updatePosition()
{
    Atom *atom1 = myStartAtom;
    Atom *atom2 = myEndAtom;

    double radius1 		= atom1->effectiveRadius();
    double radius2 		= atom2->effectiveRadius();
    double x1     		= atom1->pos().x();
    double y1     		= atom1->pos().y();
    double z1     		= atom1->zValue();
    double x2     		= atom2->pos().x();
    double y2     		= atom2->pos().y();
    double z2     		= atom2->zValue();
    double dx     		= x2-x1;
    double dy     		= y2-y1;
    double dz     		= z2-z1;
    double norm   		= sqrt(dx*dx + dy*dy);
    // Phi is the angle subtended by the bond and the y axis(which runs negative)
    double sinPhi    	= (norm == 0.0 ? 0.0 : dx/norm);
    double cosPhi       = (norm == 0.0 ? 0.0 : dy/norm);
    // The angle subtended by the bond and the z axis
    double r      		= sqrt(dx*dx + dy*dy + dz*dz);
    double sinTheta 	= norm/r;
    QPointF startPoint;
    QPointF endPoint;

    startPoint.setX(x1 + ( radius1*sinTheta*sinPhi) );
    startPoint.setY(y1 + ( radius1*sinTheta*cosPhi) );
    endPoint.setX(  x2 - ( radius2*sinTheta*sinPhi) );
    endPoint.setY(  y2 - ( radius2*sinTheta*cosPhi) );
    setLine(QLineF(startPoint, endPoint));

    // The average position is used to determine the scale factor
    // the TINY is there in case both atoms are at the same z
    // in which case we must guarantee the bond goes behind the atoms
    setZValue((atom1->zValue() + atom2->zValue())/2.0 - TINY);
    _info->getBondPen().setCapStyle( (fabs(dz) > DZ_ZERO_TOL ? Qt::RoundCap : Qt::SquareCap));
    effectiveWidth = (1.0 + _info->perspective() * zValue()) * myThickness * _info->scaleFactor();

    // A cheap and cheerful guess of the label's position
    if(myLabel != 0) {
        double length = _info->scaleFactor() * myLength;
        // Account for differing radii
        double rMidPoint = radius1 + (length - radius1 - radius2)/2.0;
        QPointF labelPos;
        labelPos.setX(myLabel->dX() + x1 + rMidPoint*sinTheta*sinPhi);
        labelPos.setY(myLabel->dY() + y1 + rMidPoint*sinTheta*cosPhi);
        myLabel->setPos(labelPos);
    }
}

void Bond::toggleDashing()
{
    if(dashedLine){
        myThickness = DEFAULT_BOND_THICKNESS;
        _info->getBondPen().setStyle(Qt::SolidLine);
        dashedLine = false;
    }else{
        myThickness = DEFAULT_DASHED_BOND_THICKNESS;
        _info->getBondPen().setStyle(Qt::CustomDashLine);
        QVector<qreal> dashes;
        qreal space = 3;
        qreal dash  = 2;
        dashes << dash << space;
        _info->getBondPen().setDashPattern(dashes);
        dashedLine = true;
    }
}

void Bond::paint(QPainter *painter,
                 const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    _info->getBondPen().setWidthF(hoverOver ? 1.5*effectiveWidth : effectiveWidth);
    _info->getBondPen().setColor(_info->getBondColor());
    painter->setBrush(_info->getBondColor());
    painter->setPen(_info->getBondPen());
    painter->drawLine(line());
    if(isSelected()) {
        _info->getBondPen().setColor(SELECTED_COLOR);
        painter->setPen(_info->getBondPen());
        painter->drawLine(line());
    }
    // Draw a semi-transparent white line for fogging
    if(_info->getUseFogging()){
        double dZ = _info->maxBondZ() - _info->minBondZ();
        double thisZ = computeMidZ() - _info->minBondZ();
        double opacity = (dZ > TINY ? 20.0*(100.0-_info->getFoggingScale())*(thisZ/dZ) : 0.0);
        opacity = (opacity < 0 ? 0 : opacity);
        opacity = (opacity > 255 ? 255 : opacity);
        _info->getBondPen().setColor(QColor(255,255,255,255-opacity));
        painter->setPen(_info->getBondPen());
        painter->drawLine(line());
    }
}
