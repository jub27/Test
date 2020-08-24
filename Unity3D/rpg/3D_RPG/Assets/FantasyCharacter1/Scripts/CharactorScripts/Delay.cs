using UnityEngine;
using System.Collections;
using System;
public class Delay1 : MonoBehaviour {
	
	public float delayTime = 1.0f;
	bool flag = false;
	long timeId;
	bool isDone = false;

	public bool checkIsDone()
	{
		return isDone;
	}

	public void resetFlag()
	{
		flag = false;
	}

	Delay1[] delayArr = null;

	void OnEnable()
	{
		if(flag)
		{
			flag = false;
			return;
		}

		if(delayArr == null)
		{
			delayArr = GetComponentsInParent<Delay1> ();
		}

		for(int i = 0; i < delayArr.Length; i++)
		{
			if(delayArr[i] == this)
			{
				continue;
			}
			if(!delayArr[i].checkIsDone())
			{
				return;
			}
		}

		flag = true;
		gameObject.SetActive (false);
        Invoke("DelayFunc", delayTime);
	}
	

	void DelayFunc()
	{
		isDone = true;
		gameObject.SetActive (true);
		//gameObject.SetActiveRecursively(true);
	}

	void OnDisable()
	{
		isDone = false;
	}
}
