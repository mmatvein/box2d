/*
* Copyright (c) 2006-2009 Erin Catto http://www.gphysics.com
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#include <Box2D/Dynamics/Contacts/b2CircleContact.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Dynamics/b2WorldCallbacks.h>
#include <Box2D/Common/b2BlockAllocator.h>
#include <Box2D/Collision/b2TimeOfImpact.h>

#include <new>

b2Contact* b2CircleContact::Create(b2Fixture* fixtureA, b2Fixture* fixtureB, b2BlockAllocator* allocator)
{
	void* mem = allocator->Allocate(sizeof(b2CircleContact));
	return new (mem) b2CircleContact(fixtureA, fixtureB);
}

void b2CircleContact::Destroy(b2Contact* contact, b2BlockAllocator* allocator)
{
	((b2CircleContact*)contact)->~b2CircleContact();
	allocator->Free(contact, sizeof(b2CircleContact));
}

b2CircleContact::b2CircleContact(b2Fixture* fixtureA, b2Fixture* fixtureB)
	: b2Contact(fixtureA, fixtureB)
{
	b2Assert(m_fixtureA->GetType() == b2Shape::e_circle);
	b2Assert(m_fixtureB->GetType() == b2Shape::e_circle);
}

void b2CircleContact::Evaluate()
{
	b2Body* bodyA = m_fixtureA->GetBody();
	b2Body* bodyB = m_fixtureB->GetBody();

	b2CollideCircles(	&m_manifold,
						(b2CircleShape*)m_fixtureA->GetShape(), bodyA->GetTransform(),
						(b2CircleShape*)m_fixtureB->GetShape(), bodyB->GetTransform());
}

float32 b2CircleContact::ComputeTOI(const b2Sweep& sweepA, const b2Sweep& sweepB) const
{
	b2TOIInput input;
	input.sweepA = sweepA;
	input.sweepB = sweepB;
	input.tolerance = b2_linearSlop;

	return b2TimeOfImpact(&input, (const b2CircleShape*)m_fixtureA->GetShape(), (const b2CircleShape*)m_fixtureB->GetShape());
}