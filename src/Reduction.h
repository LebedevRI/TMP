#ifndef HALIDE_REDUCTION_H
#define HALIDE_REDUCTION_H

/** \file
 * Defines internal classes related to Reduction Domains
 */

#include "IR.h"

namespace Halide {
namespace Internal {

/** A single named dimension of a reduction domain */
struct ReductionVariable {
    std::string var;
    Expr min, extent;
};

struct ReductionDomainContents;

/** A reference-counted handle on a reduction domain, which is just a
 * vector of ReductionVariable. */
class ReductionDomain {
    IntrusivePtr<ReductionDomainContents> contents;
public:
    /** Construct a new nullptr reduction domain */
    ReductionDomain() : contents(nullptr) {}

    /** Construct a reduction domain that spans the outer product of
     * all values of the given ReductionVariable in scanline order,
     * with the start of the vector being innermost, and the end of
     * the vector being outermost. */
    EXPORT ReductionDomain(const std::vector<ReductionVariable> &domain);

    /** Is this handle non-nullptr */
    bool defined() const {
        return contents.defined();
    }

    /** Tests for equality of reference. Only one reduction domain is
     * allowed per reduction function, and this is used to verify
     * that */
    bool same_as(const ReductionDomain &other) const {
        return contents.same_as(other.contents);
    }

    /** Immutable access to the reduction variables. */
    EXPORT const std::vector<ReductionVariable> &domain() const;

    /** Add predicate to the reduction domain. See \ref RDom::where
     * for more details. */
    EXPORT void where(const Expr &predicate);

    /** Get the predicates defined on this reducation demain if there is any. */
    EXPORT const std::vector<Expr> &predicates() const;
    EXPORT std::vector<Expr> &predicates();

    /** Return AND of predicates defined on this reducation domain as one Expr.
     * If there is no predicate (i.e. all iteration domain in this reduction domain
     * is valid), this returns an undefined Expr. */
    EXPORT Expr and_predicates() const;

    /** Mark RDom as frozen, which means it cannot accept new predicates. An
     * RDom is frozen once it is used in a Func's update definition. */
    EXPORT void freeze();

    /** Check if a RDom has been frozen. If so, it is an error to add new
     * predicates. */
    EXPORT bool frozen() const;
};

}
}

#endif
