using UnityEngine;
using System.Collections;

public class BulletRotation1:MonoBehaviour
{
	public float speed = 720f;
	public bool isX = false;
	public bool isY = true;
	public bool isZ = false;
	void Update()
	{
		if(isX)
		{
			transform.Rotate (speed * Time.deltaTime, 0, 0);
		}
		else if(isY)
		{
			transform.Rotate (0, speed * Time.deltaTime, 0);
		}
		else if(isZ)
		{
			transform.Rotate (0, 0, speed * Time.deltaTime);
		}
	}
}
