using UnityEngine;
using System.Collections;
using System.Collections.Generic;
public class PosBullet1:Bullet1
{
    public Vector3 tarPos;
    public Vector3 startPos = Vector3.zero;
	protected override void init ()
	{
		base.init ();

	}


	public override void bulleting ()
	{
		base.bulleting ();
        Vector3 attackPos = Vector3.zero;
        
        if(startPos != Vector3.zero)
        {
            attackPos = startPos;
        }
        else
        {
            attackPos = MathUtil1.findChild(player, "attackPivot").position;
        }
		transform.position = attackPos;
		//stopParticle ();
		StartCoroutine (delayBulleting ());
	}

	IEnumerator delayBulleting()
	{
		yield return null;
		bulletState = BulletState1.line;
        Vector3 attackPos = Vector3.zero;

        if (startPos != Vector3.zero)
        {
            attackPos = startPos;
        }
        else
        {
            attackPos = MathUtil1.findChild(player, "attackPivot").position;
        }
        transform.position = attackPos;
		startParticle ();
	}

	protected override void complete ()
	{
		base.complete ();
	}

	protected override void update ()
	{
		if(bulletState == BulletState1.none)
		{
			return;
		}
		base.update ();
		Vector3 attackedPos = tarPos;

        if (Vector3.SqrMagnitude(attackedPos - transform.position) <= speed * Time.deltaTime)
		{
			complete();
			return;
		}

		Vector3 pos = MathUtil1.calcTargetPosByDis (transform.position, attackedPos, speed * Time.deltaTime);
		transform.forward = pos - transform.position;
		transform.position = pos;
	}
}
