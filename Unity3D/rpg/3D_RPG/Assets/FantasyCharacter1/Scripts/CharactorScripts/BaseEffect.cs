using UnityEngine;
using System.Collections;
using System.Collections.Generic;
public class BaseEffect1:MonoBehaviour
{
	Vector3 originScale;
	void Awake()
	{
		preInit ();
	}

	protected virtual void preInit()
	{
		originScale = transform.localScale;
	}

	void Update()
	{
		update ();
	}


	protected virtual void update()
	{

	}

	public virtual void play()
	{

	}


	public virtual void stop()
	{

	}



	
	void OnDestroy()
	{

	}
}
