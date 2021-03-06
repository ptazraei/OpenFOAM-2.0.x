/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "hsCombustionThermo.H"
#include "fvMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::autoPtr<Foam::hsCombustionThermo> Foam::hsCombustionThermo::New
(
    const fvMesh& mesh
)
{
    // get model name, but do not register the dictionary
    // otherwise it is registered in the database twice
    const word modelType
    (
        IOdictionary
        (
            IOobject
            (
                "thermophysicalProperties",
                mesh.time().constant(),
                mesh,
                IOobject::MUST_READ_IF_MODIFIED,
                IOobject::NO_WRITE,
                false
            )
        ).lookup("thermoType")
    );

    Info<< "Selecting thermodynamics package " << modelType << endl;

    fvMeshConstructorTable::iterator cstrIter =
        fvMeshConstructorTablePtr_->find(modelType);

    if (cstrIter == fvMeshConstructorTablePtr_->end())
    {
        FatalErrorIn("hsCombustionThermo::New(const fvMesh&)")
            << "Unknown hsCombustionThermo type "
            << modelType << nl << nl
            << "Valid hsCombustionThermo types:" << nl
            << fvMeshConstructorTablePtr_->toc() << nl
            << exit(FatalError);
    }

    return autoPtr<hsCombustionThermo>(cstrIter()(mesh));
}


Foam::autoPtr<Foam::hsCombustionThermo> Foam::hsCombustionThermo::NewType
(
    const fvMesh& mesh,
    const word& thermoType
)
{
    // get model name, but do not register the dictionary
    // otherwise it is registered in the database twice
    const word modelType
    (
        IOdictionary
        (
            IOobject
            (
                "thermophysicalProperties",
                mesh.time().constant(),
                mesh,
                IOobject::MUST_READ_IF_MODIFIED,
                IOobject::NO_WRITE,
                false
            )
        ).lookup("thermoType")
    );

    if (modelType.find(thermoType) == string::npos)
    {
        wordList allModels = fvMeshConstructorTablePtr_->toc();
        DynamicList<word> validModels;
        forAll(allModels, i)
        {
            if (allModels[i].find(thermoType) != string::npos)
            {
                validModels.append(allModels[i]);
            }
        }

        FatalErrorIn
        (
            "autoPtr<hsCombustionThermo> hsCombustionThermo::NewType"
            "("
            "const fvMesh&, "
            "const word&"
            ")"
        )   << "Inconsistent thermo package selected:" << nl << nl
            << modelType << nl << nl << "Please select a "
            << "thermo package based on " << thermoType
            << ". Valid options include:" << nl << validModels << nl
            << exit(FatalError);
    }

    Info<< "Selecting thermodynamics package " << modelType << endl;

    fvMeshConstructorTable::iterator cstrIter =
        fvMeshConstructorTablePtr_->find(modelType);

    if (cstrIter == fvMeshConstructorTablePtr_->end())
    {
        FatalErrorIn
        (
            "autoPtr<hsCombustionThermo> hsCombustionThermo::NewType"
            "("
            "const fvMesh&, "
            "const word&"
            ")"
        )   << "Unknown hsCombustionThermo type "
            << modelType << nl << nl
            << "Valid hsCombustionThermo types are:" << nl
            << fvMeshConstructorTablePtr_->toc() << nl
            << exit(FatalError);
    }

    return autoPtr<hsCombustionThermo>(cstrIter()(mesh));
}


// ************************************************************************* //
