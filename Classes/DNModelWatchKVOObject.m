//
//  DNModelWatchKVOObject.m
//  Gateway Church and DoubleNode.com
//
//  Copyright (c) 2014 Gateway Church. All rights reserved.
//
//  Derived from work originally created by Darren Ehlers
//  Portions Copyright (c) 2012 DoubleNode.com and Darren Ehlers.
//  All rights reserved.
//

#import "DNModelWatchKVOObject.h"

@interface DNModelWatchKVOObject ()
{
    DNManagedObject*    object;
    NSArray*            attributes;
}

@end

@implementation DNModelWatchKVOObject

+ (id)watchWithModel:(DNModel*)model
           andObject:(DNManagedObject*)object
{
    return [[DNModelWatchKVOObject alloc] initWithModel:model andObject:object];
}

+ (id)watchWithModel:(DNModel*)model
           andObject:(DNManagedObject*)object
       andAttributes:(NSArray*)attributes
{
    return [[DNModelWatchKVOObject alloc] initWithModel:model andObject:object andAttributes:attributes];
}

- (id)initWithModel:(DNModel*)model
          andObject:(DNManagedObject*)pObject
{
    return [self initWithModel:model andObject:pObject andAttributes:nil];
}

- (id)initWithModel:(DNModel*)model
          andObject:(DNManagedObject*)pObject
      andAttributes:(NSArray*)pAttributes
{
    self = [super initWithModel:model];
    if (self)
    {
        object      = pObject;
        attributes  = pAttributes;
    }
    
    return self;
}

- (DNManagedObject*)object
{
    return object;
}

- (void)startWatch
{
    [super startWatch];

    if (attributes == nil)
    {
        // Track ALL attributes
        attributes  = [[[object entityDescription] attributesByName] allKeys];
    }

    [attributes enumerateObjectsUsingBlock:^(NSString* attributeName, NSUInteger idx, BOOL *stop)
     {
         [object addObserver:self
                  forKeyPath:attributeName
                     options:NSKeyValueObservingOptionOld | NSKeyValueObservingOptionNew
                     context:nil];
     }];

    [self refreshWatch];
}

- (void)cancelWatch
{
    [super cancelWatch];

    [attributes enumerateObjectsUsingBlock:^(NSString* attributeName, NSUInteger idx, BOOL *stop)
     {
         [object removeObserver:self forKeyPath:attributeName];
     }];
    
    object  = nil;
}

- (void)refreshWatch
{
    [super refreshWatch];
    
    [self executeDidChangeHandler];
}

#pragma mark - NSKeyValueObserving protocol

- (void)observeValueForKeyPath:(NSString*)keyPath
                      ofObject:(id)subjectObject
                        change:(NSDictionary*)change
                       context:(void *)context
{
    if ([change[NSKeyValueChangeNotificationIsPriorKey] isEqualToNumber:@YES])
    {
        [self executeWillChangeHandler];
    }
    else
    {
        NSIndexPath*    indexPath   = [NSIndexPath indexPathForItem:0 inSection:0];
        
        switch ([change[NSKeyValueChangeKindKey] integerValue])
        {
            case NSKeyValueChangeSetting:
            {
                if ([change[NSKeyValueChangeNewKey] isEqual:change[NSKeyValueChangeOldKey]] == NO)
                {
                    [self executeDidChangeObjectUpdateHandler:subjectObject
                                                  atIndexPath:indexPath
                                                 newIndexPath:indexPath];
                }
                break;
            }

            case NSKeyValueChangeInsertion:
            {
                [self executeDidChangeObjectInsertHandler:subjectObject
                                              atIndexPath:indexPath
                                             newIndexPath:indexPath];
                break;
            }

            case NSKeyValueChangeRemoval:
            {
                [self executeDidChangeObjectDeleteHandler:subjectObject
                                              atIndexPath:indexPath
                                             newIndexPath:indexPath];
                break;
            }

            case NSKeyValueChangeReplacement:
            {
                [self executeDidChangeObjectMoveHandler:subjectObject
                                            atIndexPath:indexPath
                                           newIndexPath:indexPath];
                break;
            }
        }

        [self executeDidChangeHandler];
    }
}

@end
