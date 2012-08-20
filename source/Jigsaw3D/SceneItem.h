#ifndef SCENEITEM_H
#define SCENEITEM_H

class SceneItem
{
public:
	SceneItem();
	virtual ~SceneItem();

	virtual void update(float p_timePassed) = 0;
	virtual void draw() const = 0;
};

#endif
