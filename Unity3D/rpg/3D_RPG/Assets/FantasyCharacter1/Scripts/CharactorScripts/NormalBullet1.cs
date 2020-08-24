using UnityEngine;
using System.Collections;
using System.Collections.Generic;
public class NormalBullet1:Bullet1
{
	protected override void init ()
	{
		base.init ();
	
	}


	public override void bulleting ()
	{
		base.bulleting ();
		Vector3 attackPos = MathUtil1.findChild(player, "attackPivot").position;
		transform.position = attackPos;
        Vector3 attackedPos = MathUtil1.findChild(target, "attackedPivot").position;
        transform.forward = attackedPos - transform.position;
        stopParticle ();
		StartCoroutine (delayBulleting ());
	}

	IEnumerator delayBulleting()
	{
		yield return null;
		bulletState = BulletState1.line;
		Vector3 attackPos = MathUtil1.findChild(player, "attackPivot").position;
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
		Vector3 attackedPos = MathUtil1.findChild(target, "attackedPivot").position;

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
