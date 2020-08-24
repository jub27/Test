using UnityEngine;
using System.Collections;
using System.Collections.Generic;
public class ActionEffectManager4:MonoBehaviour
{
    public bool isFar = true;
    public Vector3 pos;
	public List<ActionEffect2> UltimateEffect = new List<ActionEffect2>();
	public List<ActionEffect2> MagicEffect = new List<ActionEffect2>();
	public List<ActionEffect2> AttackEffect = new List<ActionEffect2>();
	public List<ActionEffect2> Magic2Effect = new List<ActionEffect2>();
	public List<ActionEffect2> getEffectByName(string str)
	{
		switch(str)
		{
		case AnimationName.Ultimate:
			return UltimateEffect;
			break;
		case AnimationName.Magic:
			return MagicEffect;
			break;
		case AnimationName.Attack:
			return AttackEffect;
			break;
		case AnimationName.Magic2:
			return Magic2Effect;
			break;
		}
		return null;
	}

	public void stopUltimateEffect()
	{
		for(int i = 0; i < UltimateEffect.Count; i++)
		{
			UltimateEffect[i].stop();
		}
	}

    void ActionDone(string actionName)
    {

    }

    void ActionStart(string actionName)
    {
        List<ActionEffect2> list = getEffectByName(actionName);
        if(list == null)
        {
            return;
        }

        for(int i = 0; i < list.Count; i++)
        {
            if(list[i] == null)
            {
                continue;
            }
            list[i].play();
        }
    }

   
}
