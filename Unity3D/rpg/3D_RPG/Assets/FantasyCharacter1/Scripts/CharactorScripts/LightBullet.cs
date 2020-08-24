using UnityEngine;
using System.Collections;
using System.Collections.Generic;
public class LightBullet1:Bullet1
{
	protected override void init ()
	{
		base.init ();
	}
	

	float color = 0;
	LineRenderer[] lineRenderArr;
	public override void bulleting ()
	{
		base.bulleting ();
		Vector3 attackPos = MathUtil1.findChild(player, "attackPivot1").position;
	
		transform.position = attackPos;
		stopParticle ();
		addLineRender ();
		StartCoroutine (delayBulleting ());
	}
	
	void addLineRender()
	{
		lineRenderArr = GetComponentsInChildren<LineRenderer>();
		for(int i = 0; i < lineRenderArr.Length; i++)
		{
			lineRenderArr[i].positionCount = 2;
		}
		color = 2f;
	}
	
	IEnumerator delayBulleting()
	{
		yield return null;
		bulletState = BulletState1.line;
		startParticle ();
		complete ();
	}
	
	protected override void complete ()
	{
		_complete();

    }
	
	void _complete()
	{
		stopParticle ();
	}
	
	
	void updateLineRender()
	{
		float speed = 1.2f;
		if(color > 0)
		{
			color -= speed * Time.deltaTime;
		}

		for(int i = 0; i < lineRenderArr.Length; i++)
		{

			lineRenderArr[i].SetPosition(0, MathUtil1.findChild(player, "attackPivot1").position);
			lineRenderArr[i].SetPosition(1, MathUtil1.findChild(target, "attackedPivot").position);
			if(i == 0)
			{
				Color c = new Color(1, 1, 1, color);
                lineRenderArr[i].startColor = c;
                lineRenderArr[i].endColor = c;
            }
			else
			{
				Color c = new Color(1, 1, 1, color);
                lineRenderArr[i].startColor = c;
                lineRenderArr[i].endColor = c;
            }
		}

		if(color <= speed)
		{
            base.complete();
            GameObject.Destroy(gameObject);
		}
	}
	
	protected override void update ()
	{
		if(bulletState == BulletState1.none)
		{
			return;
		}
		if(player == null)
		{
			GameObject.Destroy(gameObject);
			return;
		}
		base.update ();
		updateLineRender ();
	}
}

